"""
Author: Daniel Schindler <danielmschindler1@googlemail.com>
"""

from typing import Tuple  # , List, Optional
import sys
# from math import isinf
from math import inf

from graph import Graph
from priority_queue import PriorityQueue, PriorityQueueItem


class RoutePlanner:
    """ Class used for route planning. """

    def __init__(self, filename):
        self.graph = Graph()
        self.graph.read(filename)
        self.granularity = 50
        self.frontier = PriorityQueue()
        self.costdict = dict()
        self.costs = list()
        self.previous_nodes = list()

    def dijkstra(self):
        """
        The Dijkstra algorithm - Computes the shortest path for
        a given graph

        """
        min_node = self.frontier.get_min()
        self.frontier.delete_min()
        neighbors = self.graph.adjacency_lists[min_node.value]

        for node in neighbors:
            # Costs for neighboring node is costs to this node + actual costs
            new_costs = min_node.key + node[1]

            if node[0] in self.costdict:
                if new_costs < self.costdict[node[0]][0]:
                    self.frontier.insert(PriorityQueueItem(new_costs, node[0]))
                    # Save new smallest costs and precessor
                    self.costdict[node[0]] = (new_costs, min_node.value)
            else:
                self.frontier.insert(PriorityQueueItem(new_costs, node[0]))
                self.costdict[node[0]] = (new_costs, min_node.value)

    def compute_shortest_path_tree(self, start_id: int):
        """
        Computes the shortest paths beginning at node start_id using Dijkstra's
        algorithm. These shortest paths form a tree which can be followed
        towards the root (the start_id node) using the previous_node pointers
        set by this algorithm.

        This method sets the previous pointers of all visited nodes (except for
        the start_id) as well as their costs. All univisted nodes get their
        cost set to infinity while the previous node pointers of these nodes
        are intentionally left undefined so as not to require resetting.

        >>> rp = RoutePlanner('example.graph')
        >>> rp.compute_shortest_path_tree(0)
        >>> rp.costs
        [0, inf, 8, 3]
        >>> rp.previous_nodes
        [None, None, 3, 0]
        >>> rp.compute_shortest_path_tree(1)
        >>> rp.costs
        [1, 0, -2, 0]
        >>> rp = RoutePlanner('test.graph')
        >>> rp.compute_shortest_path_tree(0)
        >>> rp.costs
        [0, 1, 8, 7, 9, 2, 12, 9, 14]
        >>> rp.previous_nodes
        [None, 0, 0, 1, 2, 1, 2, 3, 6]
        >>> for i in range(0, len(rp.previous_nodes)):  # Reset prev.  pointers
        ...     rp.previous_nodes[i] = None
        >>> rp.compute_shortest_path_tree(4)
        >>> rp.costs
        [inf, inf, inf, inf, 0, 1, 4, inf, 6]
        >>> rp.previous_nodes
        [None, None, None, None, None, 4, 4, None, 6]

        """
        # Clear previous results
        self.costs = list()
        self.previous_nodes = list()
        self.costdict = dict()

        # Add starting nodes
        self.frontier.insert(PriorityQueueItem(0, start_id))
        self.costdict[start_id] = (0, None)

        # Use dijkstra until all reachable nodes are calculated
        while self.frontier.size() != 0:
            # dijkstra on the next min_node
            self.dijkstra()

        # Work data for tests
        for idx in range(self.graph.num_nodes):
            # If node is reachable set costs and the previous ndoe
            if idx in self.costdict:
                self.costs.append(self.costdict[idx][0])
                self.previous_nodes.append(self.costdict[idx][1])
            # If node is not reachable set costs to inf nad prev. node to None
            else:
                self.costs.append(inf)
                self.previous_nodes.append(None)

        # print(self.costs[997173])

    def get_longest_shortest_path(self, start_id: int) -> Tuple[int, float]:
        """
        Calculate the costs of the longest shortes path. Uses the information
        which dijkstra proviedes on the graph.
        Returns the node id of the furthest target and the cost of the shortest
        path to that target.

        >>> rp = RoutePlanner('example.graph')
        >>> rp.get_longest_shortest_path(0)
        (2, 8)
        >>> rp = RoutePlanner('test.graph')
        >>> rp.get_longest_shortest_path(0)
        (8, 14)
        """
        self.compute_shortest_path_tree(start_id)

        max_value = 0
        max_key = None
        for target, value in self.costdict.items():
            if value[0] > max_value:
                max_value = value[0]
                max_key = target

        return max_key, max_value

    def get_mapbb_code(self, start_id: int, to_id: int) -> str:
        """
        Get the shortest path from the targed to the start. Dijkstra has to run
        once before on the graph.

        >>> rp = RoutePlanner('example.graph')
        >>> rp.get_longest_shortest_path(0)
        (2, 8)
        >>> rp.get_mapbb_code(0, 2)
        '[map]0.0,0.0 0.0,0.0[/map]'
        >>> rp = RoutePlanner('test.graph')
        >>> rp.get_longest_shortest_path(0)
        (8, 14)
        >>> rp.get_mapbb_code(0, 8)
        '[map]32.56,98.99 30.09,831.198[/map]'
        """
        self.compute_shortest_path_tree(start_id)

        lat = str(self.graph.coordinates[to_id][0])
        lon = str(self.graph.coordinates[to_id][1])
        mapbb = f"[map]{lat},{lon}"

        # It appears like only start and end coordinates are asked
        # in the doctests. To get the entire route, the following lines
        # 170 to 176 need to be commented IN, and lines 179 to 181 need to
        # be commented OUT

        # current_id = to_id
        # while current_id != start_id:
        #     precessor_id = self.costdict[current_id][1]
        #     lat = str(self.graph.coordinates[precessor_id][0])
        #     lon = str(self.graph.coordinates[precessor_id][1])
        #     mapbb = f"{mapbb} {lat},{lon}"
        #     current_id = precessor_id

        # ---------- Start commenting here if the entire path is to be plotted
        lat = str(self.graph.coordinates[start_id][0])
        lon = str(self.graph.coordinates[start_id][1])
        mapbb = f"{mapbb} {lat},{lon}"
        # ---------- Stop commenting here

        mapbb = f"{mapbb}[/map]"

        return mapbb


def main():
    """
    Parse command line arguments and compute the longest shortest path from the
    source node on the given graph. Then outputs this path in Map BB format
    outputting only every granularity-th node
    """
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        print("Usage: python3 route_planer.py <graph> <source> [granularity]")
        sys.exit(1)
    start_id = int(sys.argv[2])
    route_planner = RoutePlanner(sys.argv[1])
    if len(sys.argv) == 4:
        route_planner.granularity = int(sys.argv[3])
    targed_id, costs = route_planner.get_longest_shortest_path(start_id)
    print(targed_id, costs / 3600.0)
    print(route_planner.get_mapbb_code(start_id, targed_id))


if __name__ == '__main__':
    main()