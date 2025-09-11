#include <catch2/catch_test_macros.hpp>
#include <exec/single_thread_context.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>
#include <iostream>
#include <print>
#include <stdexec/execution.hpp>
#include <syncstream>
#include <tuple>

using namespace std::literals;

TEST_CASE("senders - hello world", "[stdexec]")
{
    namespace execution = stdexec;
    using namespace exec;

    SECTION("basic usage")
    {
        static_thread_pool thread_pool{8};

        execution::scheduler auto thd_pool = thread_pool.get_scheduler();

        execution::sender auto begin = execution::schedule(thd_pool);

        execution::sender auto hi = execution::then(begin, [] {
            std::cout << "Hello, World! Have an int." << std::endl;
            return 13;
        });

        execution::sender auto add_42 = execution::then(hi, [](int arg) {
            std::cout << "Adding 42 to " << arg << std::endl;
            return arg + 42;
        });

        auto [result] = execution::sync_wait(add_42).value();

        CHECK(result == 55);
    }

    SECTION("with pipes")
    {
        static_thread_pool thread_pool{8};

        execution::scheduler auto thd_pool = thread_pool.get_scheduler();

        execution::sender auto work = execution::schedule(thd_pool)
            | execution::then([] {
                  std::cout << "Hello, World! Have an int." << std::endl;
                  return 13;
              })
            | execution::then([](int value) {
                  std::cout << "Received: " << value << std::endl;
                  return value + 42;
              });

        auto [result] = execution::sync_wait(work).value();

        CHECK(result == 55);
    }
}

TEST_CASE("switching execution contexts - threads", "[stdexec]")
{
    namespace execution = stdexec;
    using namespace exec;

    exec::single_thread_context thread_context_1;
    exec::single_thread_context thread_context_2;

    execution::scheduler auto scheduler_1 = thread_context_1.get_scheduler();
    execution::scheduler auto scheduler_2 = thread_context_2.get_scheduler();

    auto step_1 = [](int i) {
        std::cout << "Work#1 on thread " << std::this_thread::get_id() << std::endl;
        return i * 2;
    };

    auto step_2 = [](int i) {
        std::cout << "Work#2 on thread " << std::this_thread::get_id() << std::endl;
        return i + 1;
    };

    execution::sender auto work = execution::starts_on(scheduler_1, execution::just(42))
        | execution::then(step_1)
        | execution::continues_on(scheduler_2)
        | execution::then(step_2)
        | execution::continues_on(scheduler_1)
        | execution::then(step_1);

    auto [result] = execution::sync_wait(std::move(work)).value();

    CHECK(result == 170);
}

TEST_CASE("when_all", "[stdexec]")
{
    using namespace stdexec;
    using namespace exec;

    static_thread_pool thread_pool{3};

    auto scheduler = thread_pool.get_scheduler();

    auto func = [](int i) -> int {
        return i * i;
    };

    SECTION("gathers results from senders")
    {
        auto work = when_all(
            starts_on(scheduler, just(1) | then(func)),
            starts_on(scheduler, just(2) | then(func)),
            starts_on(scheduler, just(3) | then(func)));

        auto [i, j, k] = sync_wait(std::move(work)).value();

        REQUIRE(i == 1);
        REQUIRE(j == 4);
        REQUIRE(k == 9);
    }

    SECTION("throws an error if any sender sends an error")
    {
        auto work = when_all(
            starts_on(scheduler, just(1) | then(func)),
            starts_on(scheduler, just(2) | then(func)),
            starts_on(scheduler, just() | then([] { throw std::runtime_error("Error from sender!"); return 0; })));

        CHECK_THROWS_AS(sync_wait(std::move(work)), std::runtime_error);
    }

    SECTION("if any sender is stopped you must recover with let_stopped")
    {
        sender auto s1 = just(42);
        sender auto s2 = just_stopped() | let_stopped([] { return just(665); });

        auto work = when_all(s1, s2);

        REQUIRE(sync_wait(std::move(work)).value() == std::tuple{42, 665});
    }
}

TEST_CASE("splitting workflow", "[stdexec]")
{
    using namespace stdexec;
    using namespace exec;

    static_thread_pool thread_pool{8};

    auto to_upper = [](const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), [](auto c) { return std::toupper(c); });
        std::osyncstream(std::cout) << "Work#1 on thread " << std::this_thread::get_id() << std::endl;
        return result;
    };

    auto to_lower = [](const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), [](auto c) { return std::tolower(c); });
        std::osyncstream(std::cout) << "Work#2 on thread " << std::this_thread::get_id() << std::endl;
        return result;
    };

    scheduler auto sch = thread_pool.get_scheduler();

    sender auto common = just("Hello World!"s) | split();

    sender auto pipe_1 = starts_on(sch, common) | then(to_upper);
    sender auto pipe_2 = starts_on(sch, common) | then(to_lower);

    sender auto results = when_all(pipe_1, pipe_2);

    auto [upper, lower] = sync_wait(std::move(results)).value();

    REQUIRE(upper == "HELLO WORLD!");
    REQUIRE(lower == "hello world!");
}

TEST_CASE("bulk", "[stdexec]")
{
    using namespace stdexec;
    using namespace exec;

    static_thread_pool thread_pool{8};

    std::vector<int> data_1 = {1, 2, 3, 4, 5};
    std::vector<int> data_2 = {10, 20, 30, 40, 50};
    std::vector<int> results(data_1.size());

    scheduler auto sch = thread_pool.get_scheduler();

    sender auto work = schedule(sch)
        | bulk(data_1.size(), [&](size_t i) {
              std::osyncstream(std::cout) << std::format("Processing element #{} on thread {}\n", i, std::this_thread::get_id());
              results[i] = data_1[i] + data_2[i];
          });

    sync_wait(std::move(work));

    REQUIRE(results == std::vector({11, 22, 33, 44, 55}));
}

TEST_CASE("coroutines", "[stdexec][coroutines]")
{
    SECTION("using Sender as awaitable")
    {
        exec::static_thread_pool thread_pool{3};

        auto scheduler = thread_pool.get_scheduler();

        stdexec::sender auto snd = stdexec::on(scheduler, stdexec::just(8) | stdexec::then([](int i) { return i * i; }));

        auto coro_task = [snd = std::move(snd)]() -> exec::task<std::string> {
            int result = co_await snd;
            co_return "Value: "s + std::to_string(result);
        };

        auto [result] = stdexec::sync_wait(coro_task()).value();
        CHECK(result == "Value: 64");
    }
}

TEST_CASE("coroutines with error handling", "[stdexec][coroutines]")
{
    auto tsk_1 = []() -> exec::task<std::string> {
        co_return "Task 1";
    };

    auto tsk_2 = [tsk1 = std::move(tsk_1)]() -> exec::task<std::string> {
        std::string text = co_await tsk1();
        throw std::runtime_error("Error in Task 2");
        co_return text + " + Task 2";
    };

    REQUIRE_THROWS_AS(stdexec::sync_wait(tsk_2()), std::runtime_error);
}

TEST_CASE("coroutines can stop early", "[stdexec][coroutines]")
{
    int count = 0;

    auto work = [](int& count) -> exec::task<void> {
        count += 1;
        co_await [](int& count) -> exec::task<void> {
            count += 2;
            co_await stdexec::just_stopped();
            count += 4;
        }(count);
        count += 8;
    }(count);

    auto result = stdexec::sync_wait(std::move(work));

    REQUIRE(!result.has_value());
    REQUIRE(count == 3);
}

TEST_CASE("sender adaptors", "[stdexec][adaptors]")
{
    namespace ex = stdexec;

    SECTION("then - continuation")
    {
        ex::sender auto work = ex::just(42) | ex::then([](int i) { return std::to_string(i); });

        auto [result] = ex::sync_wait(std::move(work)).value();

        REQUIRE(result == "42");
    }

    // SECTION("uppon_error")
    // {
    //     const std::error_code ec = std::make_error_code(std::errc::invalid_argument);
    //
    //     ex::sender auto work =
    //         ex::just_error(ec)
    //         | ex::uppon_error([](std::error_code ec) { std::println("Error: {}", ec.value()); return 42; });
    //
    //     auto [result] = ex::sync_wait(std::move(work)).value();
    //     REQUIRE(result == "42"s);
    // }

    SECTION("let_value - monadic bind")
    {
        // we use it when value lifetime >= async operations that is performed
        // sender_of<T> | let_value(func(T) -> sender_of<U>) -> sender_of<U>

        auto to_int = [](const std::string& str) { return ex::just(std::stoi(str)); };

        ex::sender auto work = ex::just("42")
            | ex::let_value(to_int)
            | ex::then([](int i) { return std::to_string(i); });

        auto [result] = ex::sync_wait(std::move(work)).value();

        REQUIRE(result == "42");
    }

    SECTION("stopped_as_error")
    {
        int counter = 0;
        std::error_code ec = std::make_error_code(std::errc::operation_canceled);
        ex::sender auto work = ex::just(42)
            | ex::let_value([&](int value) {
                  ++counter;
                  return ex::just_stopped();
              })
            | ex::stopped_as_error(ec)
            | ex::let_error([](auto err) {
                  return ex::just("Error occured!"s);
              });

        auto [result] = ex::sync_wait(std::move(work)).value();
        REQUIRE(result == "Error occured!"s);
    }
}