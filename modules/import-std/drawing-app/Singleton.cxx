export module Singleton;

export namespace Singleton
{
    template <typename T>
    class SingletonHolder
    {
    private:
        SingletonHolder() = default;
        ~SingletonHolder() = default;

    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;

        static T& instance()
        {
            static T unique_instance;

            return unique_instance;
        }
    };
} // namespace Singleton