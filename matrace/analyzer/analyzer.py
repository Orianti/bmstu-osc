from typing import Dict, Tuple, List, Any
from numpy import max, mean, median, ndarray


class AnalyzerError(Exception):
    pass


class UnableOpenTraceFileError(AnalyzerError):
    pass


class TraceFileWrongFormatError(AnalyzerError):
    pass


class Analyzer(object):
    def __init__(self, filename: str):
        self.__trace = []
        self.__time, self.__heap, self.__stack, self.__anon = [], [], [], []
        self.load_trace_file(filename)

    def load_trace_file(self, filename: str) -> None:
        trace = []

        try:
            for line in open(filename):
                lst = line.split(' ')
                trace.append(
                    {'time': float(lst[0][1:-1]),
                     'syscall': lst[3],
                     'heap': int(lst[4]),
                     'stack': int(lst[5]),
                     'anon': int(lst[6])}
                )
        except (FileNotFoundError, PermissionError):
            raise UnableOpenTraceFileError('Can\'t open trace file')
        except (IndexError, ValueError):
            raise TraceFileWrongFormatError('Can\'t read trace file')

        self.__trace = trace
        self.__process_data()

    def get_trace(self) -> List[Dict[str, Any]]:
        return self.__trace

    def get_memory_usage_plot_data(self) -> Tuple[List[float], List[int], List[int], List[int]]:
        return self.__time, self.__heap, self.__stack, self.__anon

    def get_calls_count(self) -> int:
        return len(self.__trace)

    def get_call_frequencies(self) -> Dict[str, int]:
        frequencies = {'brk': 0, 'mmap': 0, 'munmap': 0, 'mremap': 0}

        for call in self.__trace:
            frequencies[call['syscall']] += 1

        return frequencies

    def get_max_usage(self) -> Dict[str, int]:
        return {'heap': int(max(self.__heap)), 'stack': int(max(self.__stack)), 'anon': int(max(self.__anon))}

    def get_mean_usage(self) -> Dict[str, ndarray]:
        return {'heap': mean(self.__heap), 'stack': mean(self.__stack), 'anon': mean(self.__anon)}

    def get_median_usage(self) -> Dict[str, ndarray]:
        return {'heap': median(self.__heap), 'stack': median(self.__stack), 'anon': median(self.__anon)}

    def __process_data(self) -> None:
        start_time = self.__trace[0]['time']
        self.__time = [call['time'] - start_time for call in self.__trace]

        self.__heap = [call['heap'] / 1024 for call in self.__trace]
        self.__stack = [call['stack'] / 1024 for call in self.__trace]
        self.__anon = [call['anon'] / 1024 for call in self.__trace]
