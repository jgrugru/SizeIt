#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

int calculate_size(filesystem::path proposed_file, bool quiet = false, int indent = 1)
{
    int total_bytes = 0;

    try
    {
        if (filesystem::is_regular_file(proposed_file))
        {
            float file_size = filesystem::file_size(proposed_file);
            if (!quiet)
            {
                cout << string(indent, ' ') << "\033[34m" << left << setw(100 - indent) << setfill('.') << proposed_file.string() << "\033[0m" << setfill(' ') << right << setw(16) << file_size << " bytes" << endl;
            }
            return file_size;
        }

        if (filesystem::is_directory(proposed_file))
        {
            if (!quiet)
            {
                cout << "dir \033[32m\033[3m" << proposed_file.string() << "\033[0m\n";
            }
            for (const auto &entry : filesystem::directory_iterator(proposed_file))
            {
                float file_size = calculate_size(entry, quiet);
                total_bytes += file_size;
                // std::cout << left << setw(100) << setfill('.') << entry.path().filename().string() << setfill(' ') << right << setw(16) << file_size << std::endl;
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        if (!quiet)
        {
            cerr << string(indent, ' ') << "Error accessing " << proposed_file << ": " << e.what() << endl;
        }
    }
    return total_bytes;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        cout << "*Error*: " << " Please pass in a file." << endl;
        return 1;
    }

    bool quiet = false;
    filesystem::path folder_path = argv[1];

    if (argc >= 3)
    {
        string second_arg = argv[2];
        if (second_arg == "-q")
        {
            quiet = true;
        }
    }

    if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path))
    {
        cout << "*Error*: " << folder_path << " is either a file or does not exist.";
        return 1;
    }

    int total_bytes = calculate_size(folder_path, quiet);

    float total_mb = total_bytes / 1048576.0;
    float total_gb = total_bytes / 10737418240.0;

    cout << "Total \033[93mbytes\033[0m in " << "\033[3m" << folder_path.string() << "\033[0m" << " is \033[93m" << total_bytes << " bytes\033[0m." << endl;
    cout << "Total \033[35mmegabytes\033[0m in " << "\033[3m" << folder_path.string() << "\033[0m" << " is \033[35m" << total_mb << " megabytes\033[0m." << endl;
    cout << "Total \033[36mgigabytes\033[0m in " << "\033[3m" << folder_path.string() << "\033[0m" << " is \033[36m" << total_gb << " gigabytes\033[0m." << endl;

    // uintmax_t size = std::filesystem::file_size(folder_path);

    // cout << "filesize is " << size << "\n";
    return 0;
}
