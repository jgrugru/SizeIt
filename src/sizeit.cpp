#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Arguments
{
    filesystem::path folder_path;
    bool verbose = false;
    string file_type = "*";
    string units = "mb";
    float units_multiplier = 1;

    void display() const
    {
        cout << "Folder Path: " << folder_path << endl;
        cout << "Verbose: " << (verbose ? "true" : "false") << endl;
        cout << "File Type: " << file_type << endl;
        cout << "Units: " << units << ", so setting units_mulitplier = " << units_multiplier << endl;
    }
};

float get_units_multiplier(string units)
{
    if (units == "b")
    {
        return 1;
    }
    else if (units == "mb")
    {
        return 1024.0 * 1024.0;
    }
    else if (units == "gb")
    {
        return 1024.0 * 1024.0 * 1024.0;
    }
    return 1;
}

void output_file_line(filesystem::path proposed_file, float file_size, string units, float units_multiplier, bool verbose, int indent)
{
    if (verbose)
    {
        cout << string(indent, ' ') << "\033[34m" << left << setw(100 - indent) << setfill('.') << proposed_file.string() << "\033[0m" << setfill(' ') << right << setw(16) << file_size / units_multiplier << " " << units << endl;
    }
}

int calculate_size(filesystem::path proposed_file, string file_type, string units, float units_multiplier, bool verbose = false, int indent = 1)
{
    int total_bytes = 0;

    try
    {
        if (filesystem::is_regular_file(proposed_file))
        {
            float file_size;

            if (file_type == "*")
            {
                file_size = filesystem::file_size(proposed_file);
                output_file_line(proposed_file, file_size, units, units_multiplier, verbose, indent);
                return file_size;
            }

            string proposed_file_suffix = proposed_file.extension().string();

            if (proposed_file_suffix == file_type)
            {
                file_size = filesystem::file_size(proposed_file);
                output_file_line(proposed_file, file_size, units, units_multiplier, verbose, indent);
                return file_size;
            }
        }

        if (filesystem::is_directory(proposed_file))
        {
            if (verbose)
            {
                cout << "dir \033[32m\033[3m" << proposed_file.string() << "\033[0m\n";
            }
            for (const auto &entry : filesystem::directory_iterator(proposed_file))
            {
                float file_size = calculate_size(entry, file_type, units, units_multiplier, verbose, indent);
                total_bytes += file_size;
                // std::cout << left << setw(100) << setfill('.') << entry.path().filename().string() << setfill(' ') << right << setw(16) << file_size << std::endl;
            }
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        if (verbose)
        {
            cerr << string(indent, ' ') << "Error accessing " << proposed_file << ": " << e.what() << endl;
        }
    }
    return total_bytes;
}

Arguments parse_args(int argc, char **argv)
{
    Arguments args;

    if (argc == 1)
    {
        cout << "*Error*: " << " Please pass in a file." << endl;
        exit(1);
    }
    args.folder_path = argv[1];

    if (!filesystem::exists(args.folder_path) || !filesystem::is_directory(args.folder_path))
    {
        cout << "*Error*: " << args.folder_path << " is either a file or does not exist.";
        exit(1);
    }

    if (argc >= 3)
    {
        for (int i = 0; i < argc; i++)
        {
            // cout << "arg " << i << ": " << argv[i] << endl;
            if (string(argv[i]) == "-v")
            {
                args.verbose = true;
            }

            if (string(argv[i]) == "--file-type")
            {
                if (i + 1 < argc)
                {
                    args.file_type = argv[i + 1];
                }
                else
                {
                    cout << "Missing value for '--file-type' argument." << endl;
                    exit(1);
                }
            }

            if (string(argv[i]) == "--units")
            {
                if (i + 1 < argc)
                {
                    args.units = argv[i + 1];
                    if (args.units != "b" && args.units != "mb" && args.units != "gb")
                    {
                        cout << "Invalid value for '--units' argument. Type is LiteralStr['b', 'mb', 'gb']" << endl;
                        exit(1);
                    }
                }
                else
                {
                    cout << "Missing value for '--units' argument." << endl;
                    exit(1);
                }
            }
        }
    }

    args.units_multiplier = get_units_multiplier(args.units);
    return args;
}

int main(int argc, char **argv)
{
    Arguments args = parse_args(argc, argv);
    // args.display();

    int total_bytes = calculate_size(args.folder_path, args.file_type, args.units, args.units_multiplier, args.verbose);
    float total_mb = total_bytes / (1024.0 * 1024.0);
    float total_gb = total_bytes / (1024.0 * 1024.0 * 1024.0);

    cout << "Total \033[93m" << args.units << "\033[0m in " << "\033[3m" << args.folder_path.string() << "\033[0m" << " is \033[93m" << total_bytes / args.units_multiplier << " " << args.units << "\033[0m." << endl;

    return 0;
}

// default to megabytes, allow picking which size selector
// allow verbose mode
// allow picking a certain file type