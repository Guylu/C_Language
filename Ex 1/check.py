import os
import sys
import subprocess


def getInputFiles():
    f = sys.argv[1]
    with open(f) as file:
        return file.readlines()


def runTests():
    lines = getInputFiles()
    name = sys.argv[2]
    for line in lines:
        if len(line) > 1:
            print(line[0:-1])
            #print("'the files are identical' is good. 2 'ERROR' messages in a row is good. 'the files differ' is bad, only 1 'ERROR' message is bad.")
            os.system("~proglab/www/c_ex1/DrumExperiment < " + line[0:-1] + " >& school_out_" + line[0:-1])
            os.system(name + " < " + line[0:-1] + " >& my_out_"+ line[0:-1])
            #os.system("diff -q -s school_out_"+line[0:-1] + " my_out_"+ line[0:-1])
            with open("my_out_"+line[0:-1]) as file:
                my = file.readlines()
                with open("school_out_"+line[0:-1]) as file2:
                    school = file2.readlines()
                    file.close()
                    file2.close()
            #        print(my)
            #        print(school)
            output = subprocess.check_output(["diff", "-q", "-s", "school_out_"+line[0:-1], "my_out_"+line[0:-1]])
            if "identical" in str(output):
                print("Passed!")
            else:
                print("Failed :(")
                os.system("diff school_out_"+line[0:-1] + "my_out_"+line[0:-1])
            print()

runTests()
