import os
import subprocess

import random

import numpy as np
from matplotlib import pyplot
from scipy.interpolate import interp1d

algs = {}
if __name__ == '__main__':
    if not os.path.exists('build'):
        os.system('make all')
    
    process = subprocess.Popen(
        [ 'ls', 'build' ],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, stderr = process.communicate(input="".encode())
    files = [ x.split('.')[0].split('_') for x in stdout.decode().strip().split('\n') ]
    for file in files:
        if not algs.get(file[1]):
            algs[file[1]] = []
        algs[file[1]].append(file[0])

    print('Avaible categories are:')
    for cat in algs.keys():
        print(f'\t- {cat}')

    category = None
    while category is None or category not in algs.keys():
        if not category is str:
            print("This category is not supported")
        category = str(input('Insert a category by its name: '))

    chossen_algs: list = algs[category]
    print(f'Executing algoritms of {category}: {', '.join(chossen_algs)}')
    
    results = {
        'best': {'x': [0]},
        'average': {'x': [0]},
        'worst': {'x': [0]}
    }

    for alg in chossen_algs:
        for case in ['best', 'average', 'worst']:
            if alg not in results[case]:
                results[case][alg] = {'y': [0]}

    # Problem sizes
    problem_sizes = [1, 10, 100, 1000, 10000, 50000, 100000, 500000, 1000000]

    # Case loop
    for case in ['best', 'average', 'worst']:
        for n in problem_sizes:
            if case == 'best':
                input_data = ' '.join(str(i + 1) for i in range(n))
            elif case == 'average':
                data = list(range(n))
                random.shuffle(data)
                input_data = ' '.join(str(i + 1) for i in data)
            elif case == 'worst':
                input_data = ' '.join(str(i) for i in range(n, 0, -1))

            results[case]['x'].append(n)

            for alg in chossen_algs:
                process = subprocess.Popen(
                    [f'./build/{alg}_{category}', str(n)],
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
                )
                stdout, stderr = process.communicate(input=input_data.encode())
                output = stdout.decode().strip()

                # Handle invalid or missing output
                if not output or len(output.split()) < 2:
                    print(f"[Warning] Invalid output for {alg}, {case} case, n={n}: '{output}'")
                    results[case][alg]['y'].append(0)
                    continue

                start_str, end_str = output.split()
                start, end = int(start_str), int(end_str)
                results[case][alg]['y'].append(end - start)

        # Plotting each case
        pyplot.figure(case.capitalize() + ' Case')
        pyplot.title(f'Comparison of {category} algorithms: {case.capitalize()} Case')
        pyplot.ylabel('Execution time [ms]')
        pyplot.xlabel('Amount of numbers')
        pyplot.xscale('log')

        xnew = np.linspace(results[case]['x'][0], results[case]['x'][-1], num=500, endpoint=True)
        for alg in chossen_algs:
            f = interp1d(results[case]['x'], results[case][alg]['y'], kind='cubic', fill_value="extrapolate")
            pyplot.plot(xnew, f(xnew), label=alg)

        pyplot.legend(loc='best')

    # Show all plots
    pyplot.show()
    os.system('make clean')