from typing import Dict, Tuple


class Analyzer(object):
    def __init__(self, filename: str):
        self.__trace = []
        self.load_trace_file(filename)

    def load_trace_file(self, filename: str) -> None:
        trace = []

        for line in open(filename):
            lst = line.split(' ')
            trace.append(
                {'time': float(lst[0][1:-1]),
                 'syscall': lst[3],
                 'heap': int(lst[4]),
                 'stack': int(lst[5]),
                 'anon': int(lst[6])}
            )

        self.__trace = trace

        # for call in self.__trace:
        #     print(call)

    def get_memory_usage_plot_data(self) -> Tuple[list, list, list, list]:
        start_time = self.__trace[0]['time']
        time = [call['time'] - start_time for call in self.__trace]

        heap = [call['heap'] for call in self.__trace]
        stack = [call['stack'] for call in self.__trace]
        anon = [call['anon'] for call in self.__trace]

        return time, heap, stack, anon

    def get_call_frequencies(self) -> Dict[str, int]:
        frequencies = {'brk': 0, 'mmap': 0, 'munmap': 0, 'mremap': 0}

        for call in self.__trace:
            frequencies[call['syscall']] += 1

        return frequencies
