import os
import time
import subprocess
from matplotlib import pyplot
from scipy.interpolate import interp1d
import numpy as np

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
    
    results = {}
    for alg in chossen_algs:
        results['x'] = [ 0 ]
        results[alg] = {}
        results[alg]['y'] = [ 0 ]

    for n in [ 1, 10, 100, 1000, 10000, 50000, 100000, 500000, 1000000 ]:
        input_data: str = ' \n'.join(str(i + 1) for i in range(n))
        results['x'].append(n)

        for alg in chossen_algs:
            process = subprocess.Popen(
                [ f'./build/{alg}_{category}', str(n) ],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            stdout, stderr = process.communicate(input=input_data.encode())
            output = stdout.decode().strip()

            start_str, end_str = output.split()
            start, end = int(start_str), int(end_str)
            results[alg]['y'].append(end - start)

    xnew = np.linspace(results['x'][0], results['x'][-1], num=500, endpoint=True)
    for alg in chossen_algs:
        f = interp1d(results['x'], results[alg]['y'], kind='cubic', fill_value="extrapolate")
        pyplot.plot(xnew, f(xnew))
    
    pyplot.legend(chossen_algs, loc='best')
    pyplot.title(f'Comparation of {category} algorithms: Worst Case')
    pyplot.ylabel('Execution time [ms]')
    pyplot.xlabel('Amount of numbers')
    pyplot.xscale('log')
    pyplot.show()
    os.system('make clean')