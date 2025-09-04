import std;
import std.compat;
 
int main()
{
    std::print("{0} {2}{1}!\n", "Hello", 23, "C++"); // overload (2)
 
    const auto tmp {std::filesystem::temp_directory_path() / "test.txt"};
 
    if (FILE* stream{fopen(tmp.string().c_str(), "w")})
    {
        std::print(stream, "File: {}", tmp.string()); // overload (1)
        fclose(stream);
    }
}