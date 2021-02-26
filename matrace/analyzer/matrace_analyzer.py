#!/usr/bin/python3

import argparse
import sys
from typing import List, Tuple
import matplotlib.pyplot as plt

from analyzer import Analyzer
from analyzer import UnableOpenTraceFileError, TraceFileWrongFormatError


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='matrace analyzer')

    parser.add_argument('filenames', metavar='filename', type=str, nargs='+',
                        help='trace file names')

    return parser.parse_args()


def print_stat(analyzer: Analyzer) -> None:
    frequencies = analyzer.get_call_frequencies()
    print(f'brk calls:    {frequencies["brk"]}')
    print(f'mmap calls:   {frequencies["mmap"]}')
    print(f'munmap calls: {frequencies["munmap"]}')
    print(f'Total calls:  {analyzer.get_calls_count()}')

    max_usage = analyzer.get_max_usage()
    print(f'Max usage:    '
          f'heap: {max_usage["heap"]}K, stack: {max_usage["stack"]}K, anon: {max_usage["anon"]}K')

    mean_usage = analyzer.get_mean_usage()
    print(f'Mean usage:   '
          f'heap: {mean_usage["heap"]:.2f}K, stack: {mean_usage["stack"]:.2f}K, anon: {mean_usage["anon"]:.2f}K')


def create_plots(data: List[Tuple[Tuple[List[float], List[int], List[int], List[int]], str]]) -> None:
    colors = ['#2CBDFE', '#47DBCD', '#F3A0F2', '#9D2EC5', '#661D98', '#F5B14C']
    plt.rcParams['axes.prop_cycle'] = plt.cycler(color=colors)

    heap_fig = plt.figure(num='matrace-analyzer (heap usage)')
    stack_fig = plt.figure(num='matrace-analyzer (stack usage)')
    anon_fig = plt.figure(num='matrace-analyzer (anon page usage)')

    heap_axes = heap_fig.gca(title='Heap usage', xlabel='Time (sec)', ylabel='Memory (kilobytes)')
    stack_axes = stack_fig.gca(title='Stack usage', xlabel='Time (sec)', ylabel='Memory (kilobytes)')
    anon_axes = anon_fig.gca(title='Anon page usage', xlabel='Time (sec)', ylabel='Memory (kilobytes)')

    for d in data:
        (time, heap, stack, anon), filename = d
        heap_axes.plot(time, heap, label=filename)
        stack_axes.plot(time, stack, label=filename)
        anon_axes.plot(time, anon, label=filename)

    heap_axes.legend()
    stack_axes.legend()
    anon_axes.legend()

    plt.show()


def main(filenames: List[str]) -> int:
    data = []

    for filename in filenames:
        try:
            analyzer = Analyzer(filename)
        except UnableOpenTraceFileError:
            print(f'  File {filename} does not exist or cannot be accessed.')
            continue
        except TraceFileWrongFormatError:
            print(f'  File {filename} the file is not in the correct format.')
            continue

        data.append((analyzer.get_memory_usage_plot_data(), filename))

        print(f'  Trace file {filename}:')
        print_stat(analyzer)

    if len(data) == 0:
        return 1

    create_plots(data)

    return 0


if __name__ == '__main__':
    args = parse_args()

    sys.exit(main(args.filenames))
