"""
Author: Daniel Schindler <danielmschindler1@googlemail.com>
"""

class Graph:

    def __init__(self):
        """ Create an empty graph. """

        self.num_nodes = 0
        self.num_edges = 0
        self.coordinates = []
        self.adjacency_lists = []

    def read(self, file_name):
        """
        Read graph from given file.

        >>> graph = Graph()
        >>> graph.read("example-graph-1.txt")
        >>> graph
        [0->3|3, 1->0|1, 1->2|-2, 2->3|3, 3->2|5]
        """

        with open(file_name) as file:
            self.num_nodes = int(file.readline())
            self.num_edges = int(file.readline())
            self.adjacency_lists = [[] for u in range(self.num_nodes)]
            for line in file.readlines():
                if not line.startswith("#"):
                    if len(line.split("\t")) > 2:
                        u, v, cost = line.split("\t")
                        u, v, cost = int(u), int(v), int(cost)
                        self.adjacency_lists[u].append((v, cost))
                    else:
                        lat, lon = line.split("\t")
                        self.coordinates.append((float(lat.strip("\n")),
                                                 float(lon.strip("\n"))))

    def __repr__(self):
        """
        The graph as human-readable string (for debugging and testing).

        >>> graph = Graph()
        >>> graph.num_nodes = 4
        >>> graph.num_edges = 5
        >>> graph.adjacency_lists = [[(3, 3)], [(0, 1), (2, -2)], [(3,3)], [(2,
        ... 5)]]
        >>> graph
        [0->3|3, 1->0|1, 1->2|-2, 2->3|3, 3->2|5]
        """

        edge_strings = []
        for u in range(self.num_nodes):
            for (v, cost) in self.adjacency_lists[u]:
                edge_strings.append("%d->%d|%d" % (u, v, cost))
        return "[" + ", ".join(edge_strings) + "]"