from pyspark import SparkContext
import Sliding, argparse

def solve_puzzle(master, output, height, width, slaves):
    global HEIGHT, WIDTH, level
    HEIGHT=height
    WIDTH=width
    level = 0

    sc = SparkContext(master, "python")

    """ YOUR CODE HERE """

    sc.stop()



""" DO NOT EDIT PAST THIS LINE

You are welcome to read through the following code, but you
do not need to worry about understanding it.
"""

def main():
    """
    Parses command line arguments and runs the solver appropriately.
    If nothing is passed in, the default values are used.
    """
    parser = argparse.ArgumentParser(
            description="Returns back the entire solution graph.")
    parser.add_argument("-M", "--master", type=str, default="local[8]",
            help="url of the master for this job")
    parser.add_argument("-O", "--output", type=str, default="solution-out",
            help="name of the output file")
    parser.add_argument("-H", "--height", type=int, default=2,
            help="height of the puzzle")
    parser.add_argument("-W", "--width", type=int, default=2,
            help="width of the puzzle")
    parser.add_argument("-S", "--slaves", type=int, default=6,
            help="number of slaves executing the job")
    args = parser.parse_args()

    global PARTITION_COUNT
    PARTITION_COUNT = args.slaves * 16

    # call the puzzle solver
    solve_puzzle(args.master, args.output, args.height, args.width, args.slaves)

# begin execution if we are running this file directly
if __name__ == "__main__":
    main()
