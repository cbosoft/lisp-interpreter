from random import random, randint
import subprocess as sp
import sys
import re


FG_BLACK   = "\033[30m"
FG_RED     = "\033[31m"
FG_GREEN   = "\033[32m"
FG_YELLOW  = "\033[33m"
FG_BLUE    = "\033[34m"
FG_MAGENTA = "\033[35m"
FG_CYAN    = "\033[36m"
FG_WHITE   = "\033[37m"

BG_BLACK   = "\033[40m"
BG_RED     = "\033[41m"
BG_GREEN   = "\033[42m"
BG_YELLOW  = "\033[43m"
BG_BLUE    = "\033[44m"
BG_MAGENTA = "\033[45m"
BG_CYAN    = "\033[46m"
BG_WHITE   = "\033[47m"

RESET      = "\033[0m"
BOLD       = "\033[1m"
DIM        = "\033[2m"
ITALIC     = "\033[3m"
UNDERLINE  = "\033[4m"
BLINK      = "\033[5m"
INVERTED   = "\033[7m"

def runsh(command):
    pr = sp.Popen(command, shell=True, stdout=sp.PIPE, stderr=sp.PIPE)
    rv = pr.wait()
    so = pr.stdout.read().decode()
    if so:
        so = so[:-1]
    se = pr.stderr.read().decode()
    if se:
        se = se[:-1]
    return rv, so, se

class TestRunner:

    def __init__(self):
        self.tests = list()

    def add_test(self, name, command, expected_output, expected_error, expected_rv):
        self.tests.append( (name, command, expected_output, expected_error, expected_rv) )

    def add_many(self, group_name, inputs, operations):
        for input_name, left, right in inputs:
            for op, f in operations:
                self.add_test(f'{group_name} ({input_name}, {op})', f'({op} {left} {right})', re.compile(str(f(left, right))), nothing_re, 0)

    def add_many_fail(self, group_name, inputs, operations, error, rv):
        for input_name, left, right in inputs:
            for op, f in operations:
                self.add_test(f'{group_name} ({input_name}, {op})', f'({op} {left} {right})', nothing_re, err, rv)

    def run_tests(self):
        for name, command, expected_output, expected_error, expected_rv in self.tests:
            rv, output, error = runsh(f'./crisp -c "{command}"')
            #print(f'{rv}({expected_rv}), {output}({expected_output}), {error}({expected_error})')
            print(f"{name} --- ", end='')
            if (rv == expected_rv) and (expected_output.match(output)) and (expected_error.match(error)):
                print(f"{FG_GREEN}PASS{RESET}")
            else:
                print(f"{BG_RED}FAIL{RESET}")

if __name__ == "__main__":
    regular_int1 = randint(1, 100)
    regular_int2 = randint(1, 100)
    big_int1 = randint(sys.maxsize/3, sys.maxsize/2)
    big_int2 = randint(sys.maxsize/3, sys.maxsize/2)
    string1 = '"abc"'
    string2 = '"123"'

    one = 1
    zero = 0
    
    divbyzero_re = re.compile('.*DivideByZeroError.*')
    nothing_re = re.compile('')
    
    test_cases = list()
    arithmetic_operations = [
            ('add', lambda l,r: l+r), 
            ('+', lambda l,r: l+r), 
            ('subtract', lambda l,r: l-r),
            ('-', lambda l,r: l-r),
            ('multiply', lambda l,r: l*r),
            ('*', lambda l,r: l*r),
            ('divide', lambda l,r: l//r),
            ('/', lambda l,r: l//r)
        ]

    arithmetic_inputs = [
            ('regular int', regular_int1, regular_int2), 
            ('big int', big_int1, big_int2)
        ]

    arithmetic_fails = [
            ('strings', string1, string2)
        ]
    
    test_runner = TestRunner()
    test_runner.add_many('arithmetic', arithmetic_inputs, arithmetic_operations)
    test_runner.add_test('arithmetic (zero, /)', '(/ 1 0)', nothing_re, divbyzero_re, 1)

    boolean_operations = [
            ('or', lambda l,r: 't' if l or r else 'nil'), 
            ('||', lambda l,r: 't' if l or r else 'nil'),
            ('and', lambda l,r: 't' if l and r else 'nil'),
            ('&&', lambda l,r:'t' if l and r else 'nil')
        ]
    boolean_inputs = [
            ('both', one, one),
            ('left only', one, zero),
            ('right only', zero, one),
            ('neither', zero, zero)
        ]
    test_runner.add_many('boolean', boolean_inputs, boolean_operations)
    
    test_runner.run_tests()



