"""
Tester for OOP, exercise 5.
"""
import textwrap
from os import listdir
# from os import startfile
from os.path import isfile, join, isdir, exists
from os import path
import subprocess

tester_ver = 1.3
ASCII_ERROR = """
______  _______  _______  _______  _______ 
(  ____ \(  ____ )(  ____ )(  ___  )(  ____ )
| (    \/| (    )|| (    )|| (   ) || (    )|
| (__    | (____)|| (____)|| |   | || (____)|
|  __)   |     __)|     __)| |   | ||     __)
| (      | (\ (   | (\ (   | |   | || (\ (   
| (____/\| ) \ \__| ) \ \__| (___) || ) \ \__
(_______/|/   \__/|/   \__/(_______)|/   \__/
"""
ASCII_PASSES = """
  , ; ,   .-'^^^'-.   , ; ,
  \\|/  .'         '.  \|//
   \-;-/   ()   ()   \-;-/
   // ;               ; \\
  //__; :.         .; ;__\\
 `-----\'.'-.....-'.'/-----'
        '.'.-.-,_.'.'
          '(  (..-'
            '-'
"""

# paths to files and folder
path_to_test_files = path.join("tester_files")
path_to_tests = path.join(path_to_test_files, "good_input_to_test")
path_to_arror_tests = path.join(path_to_test_files, "error_input_to_test")
path_to_system_out = path.join(path_to_test_files, "system_out")
path_to_user_out = path.join(path_to_test_files, "user_out")

path_to_c_files = path.join("DrumExperiment.c")
path_to_cMake = path.join("CMakeLists.txt")
path_to_compiled_files = path.join("ex1.exe")

# name of files
name_of_user_output_file_no_folder = "_user" + "_output" + ".txt"
name_of_user_errors_file_no_folder = "_user" + "_errors" + ".txt"
name_of_commands_file = "commands.txt"
name_of_school_solution_output_no_folder = "_school_solution" + "_output" + ".txt"
name_of_school_solution_errors_no_folder = "_school_solution" + "_errors" + ".txt"


def can_not_test(msg):
    print("Error:", msg)
    input("press Enter to exit")
    exit(1)


def run_with_cmd(command_list, str=""):
    """
    Execute the given command list with the command line
    Return a tuple containing the return code, output and errors.
    """

    process = subprocess.Popen(command_list, shell=True, stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

    process.communicate(str)
    output, errors = process.communicate()
    return process.returncode, output, errors


def print_with_indentation(title, to_print):
    """print text in a nice way"""
    prefix = title + ": "
    wrapper = textwrap.TextWrapper(initial_indent=prefix, break_long_words=False,
                                   subsequent_indent=' ' * len(prefix))
    print(wrapper.fill(to_print))


def compile_file():
    """
    compile the java files. the compiled files are located in \test_files\compiled_files.
    terminate the tester if there was an error.
    :return: true if was successful.
    """
    if not exists(path_to_c_files):
        can_not_test("You don't have the folder: " + path_to_c_files)


    command_list = ["gcc", "-std=c99", "-lm", path_to_c_files, "-o", "ex1"]

    code, output, errors = run_with_cmd(command_list)
    if code != 0:
        can_not_test("problem with compiling\n"+"error message\n"+errors)
    print("compile OK\n\n")


def run_one_test(test_folder_name):
    """
    run one test. run your code with command file given and with the data given. sva the rusolts in a txt file.
    then compare it to the school solution txt file.
    :param test_folder_name: the name of the folder of the test (tests/test#)
    :param files_to_filter_folder: the data folder  (simple of complex)
    :return: true if was successful.
    """
    path_to_test_folder = path.join(path_to_tests, test_folder_name)

    name_of_school_solution_output = test_folder_name + name_of_school_solution_output_no_folder
    name_of_school_solution_errors = "empty.txt"

    path_to_school_solution_output = path.join(path_to_system_out, name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out, name_of_school_solution_errors)

    name_of_user_output = test_folder_name + name_of_user_output_file_no_folder
    name_of_user_errors = test_folder_name + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)


    print("starting", test_folder_name, "..")

    with open(path_to_test_folder, 'r') as intput_file:
        str = intput_file.read()

    command_list = [path_to_compiled_files]

    code, user_output, user_errors = run_with_cmd(command_list, str)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output, path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors, path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True


def run_one_error_test(test_folder_name):
    """
    run one test. run your code with command file given and with the data given. sva the rusolts in a txt file.
    then compare it to the school solution txt file.
    :param test_folder_name: the name of the folder of the test (tests/test#)
    :param files_to_filter_folder: the data folder  (simple of complex)
    :return: true if was successful.
    """
    path_to_test_folder = path.join(path_to_arror_tests, test_folder_name)

    name_of_school_solution_output = "empty.txt"
    name_of_school_solution_errors = "error.txt"

    path_to_school_solution_output = path.join(path_to_system_out, name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out, name_of_school_solution_errors)

    name_of_user_output = test_folder_name + name_of_user_output_file_no_folder
    name_of_user_errors = test_folder_name + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)


    print("starting", test_folder_name, "..")

    with open(path_to_test_folder, 'r') as intput_file:
        str = intput_file.read()

    command_list = [path_to_compiled_files]

    code, user_output, user_errors = run_with_cmd(command_list, str)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output, path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors, path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True

def compare_files(file1, file2):
    """
    compare to files with FC (windows file comparer)
    :param file1:
    :param file2:
    :return: the compaction text if there was errors
    """
    command_to_compare = ['fc', '/W', '/N', '/A', file1, file2]
    code, output, errors = run_with_cmd(command_to_compare)

    if code != 0:  # if code != 0
        return output
    return None


def run_tests():
    """
    run all the test in the folder test_files\tests with both data folders (simple, complex)
    :return: true iff all passed
    """
    all_passed = True
    tests_names = [t for t in listdir(path_to_tests)]  # list of test names
    error_test = [t for t in listdir(path_to_arror_tests)]
    number_of_tests = len(tests_names) + len(error_test)
    passed_tests = 0
    for test_name in tests_names:  # each test
        if run_one_test(test_name):
            passed_tests += 1
        else:
            all_passed = False
        print()
    for test_name in error_test:  # each test
        if run_one_error_test(test_name):
            passed_tests += 1
        else:
            all_passed = False
        print()

    if all_passed:
        print("All tests passed!!")
        return True
    else:
        print("passes", passed_tests, "out of", number_of_tests, "tests")
        return False


def passed_all():
    print(ASCII_PASSES)
    print("you passed everything!!! \ngo get some sleep")
    # startfile(name_of_p_file)


if __name__ == "__main__":
    while True:
        print("starting tester for ex5 version", tester_ver, '\n')
        compile_file()
        tests_passed = run_tests()

        if tests_passed:
            passed_all()
        else:
            print(ASCII_ERROR)

        input("press enter to restart the tester")
        print('\n\n\nRestarting...')
