"""
Author: Daniel Schindler <danielmschindler1@googlemail.com>
"""

from typing import List, Tuple, Any


class PriorityQueue:
    """ A priority queue """

    def __init__(self, to_heapify: List[Tuple[Any, Any]] = None):
        """
        Constructs the PriorityQueue from the given list to_heapify containing
        (key, value) pairs so that after initialization the PriorityQueue
        fullfills the heap property. If to_heapify is None (as in the default)
        or [] an empty PriorityQueue should be constructed.

        This is done by using the so called heapify procedure. It first
        converts the items to the correct item type.  Then the heap property is
        restored by successively repairing it on all inner nodes starting from
        the last parent and proceeding in backwards level order.  The entire
        procedure runs in O(n) time where n is the length of the list being
        heapified.

        >>> PriorityQueue()
        [None]
        >>> PriorityQueue([])
        [None]
        >>> pq = PriorityQueue([(7, 'B'), (2, 'C'), (3, 'Q')])
        >>> pq
        [None, 2C@1, 7B@2, 3Q@3]
        """
        if not to_heapify:
            self.items = [None]
            return
        # Insert the dummy element for the 1 based indexing
        to_heapify.insert(0, None)
        # Convert elements to PriorityQueueItems
        for i in range(1, len(to_heapify)):
            key, value = to_heapify[i]
            to_heapify[i] = PriorityQueueItem(key, value)
            to_heapify[i].heap_index = i
        self.items = to_heapify
        # Start at the last parent which is parent of the last element
        # (remember this time we are dealing with 1 based indexing)
        curr = self.size() // 2
        while curr >= 1:
            self.repair_heap_downwards(curr)
            curr -= 1

    def size(self):
        """ The number of items in the PQ.

        >>> pq = PriorityQueue()
        >>> pq.size()
        0
        """
        return len(self.items) - 1

    def change_key(self, item: 'PriorityQueueItem'):
        """
        After an items key was updated change_key() restores the heap property

        >>> pq = PriorityQueue()
        >>> pq.insert(PriorityQueueItem(3, "Q"))
        >>> pq.insert(PriorityQueueItem(7, "B"))
        >>> to_change = PriorityQueueItem(2, "C")
        >>> pq.insert(to_change)
        >>> pq.insert(PriorityQueueItem(1, "X"))
        >>> to_change.key = 0
        >>> pq.change_key(to_change)
        >>> pq
        [None, 0C@1, 1X@2, 3Q@3, 7B@4]
        """
        idx = item.heap_index
        parent = idx // 2
        child_idx = 2 * idx
        if self.items[parent].key > self.items[idx].key:
            self.repair_heap_upwards(idx)
        elif child_idx < len(self.items) and \
                (self.items[idx].key < self.items[child_idx].key or
                 self.items[idx].key < self.items[child_idx + 1].key):
            self.repair_heap_downwards(idx)

    def remove(self, item: 'PriorityQueueItem'):
        """
        Remove an item from the PQ given a reference

        >>> pq = PriorityQueue()
        >>> pq.insert(PriorityQueueItem(3, "Q"))
        >>> pq.insert(PriorityQueueItem(7, "B"))
        >>> to_remove = PriorityQueueItem(2, "C")
        >>> pq.insert(to_remove)
        >>> pq.insert(PriorityQueueItem(1, "X"))
        >>> pq.remove(to_remove)
        >>> pq
        [None, 1X@1, 7B@2, 3Q@3]
        >>> pq = PriorityQueue()
        >>> to_remove = PriorityQueueItem(3, "Q")
        >>> pq.insert(to_remove)
        >>> pq.remove(to_remove)
        >>> pq
        [None]
        """
        idx = item.heap_index
        if idx == len(self.items) - 1:
            # We can just remove the last element
            del self.items[-1]
            return
        move = self.items[-1]
        del self.items[-1]
        self.items[idx] = move
        move.heap_index = idx
        self.change_key(move)

    def delete_min(self):
        """
        Delete minimum item from the heap while ensuring the heap property.

        >>> pq = PriorityQueue()
        >>> pq.insert(PriorityQueueItem(3, "Q"))
        >>> pq.insert(PriorityQueueItem(7, "B"))
        >>> pq.insert(PriorityQueueItem(2, "C"))
        >>> pq.insert(PriorityQueueItem(1, "X"))
        >>> pq.delete_min()
        >>> pq
        [None, 2C@1, 7B@2, 3Q@3]
        """
        if self.size() == 0:
            return
        self.swap_items(1, self.size())
        self.items.pop()
        self.repair_heap_downwards(1)

    def get_min(self):
        """ Get item with minimal key.

        >>> pq = PriorityQueue()
        >>> pq.insert(PriorityQueueItem(3, "Q"))
        >>> pq.insert(PriorityQueueItem(7, "B"))
        >>> pq.insert(PriorityQueueItem(2, "C"))
        >>> pq.insert(PriorityQueueItem(1, "X"))
        >>> pq.get_min()
        1X@1
        """
        if self.size() == 0:
            return None
        else:
            return self.items[1]

    def __repr__(self):
        """ A priority queue in human-readable form. """
        return str(self.items)

    def insert(self, item):
        """
        Insert given item (create with PriorityQueueItem below).

        >>> pq = PriorityQueue()
        >>> pq
        [None]
        >>> pq.insert(PriorityQueueItem(3, "Q"))
        >>> pq
        [None, 3Q@1]
        >>> pq.insert(PriorityQueueItem(7, "B"))
        >>> pq
        [None, 3Q@1, 7B@2]
        >>> pq.insert(PriorityQueueItem(2, "C"))
        >>> pq
        [None, 2C@1, 7B@2, 3Q@3]
        >>> pq.insert(PriorityQueueItem(1, "X"))
        >>> pq
        [None, 1X@1, 2C@2, 3Q@3, 7B@4]
        """
        self.items.append(item)
        item.heap_index = len(self.items) - 1
        self.repair_heap_upwards(item.heap_index)

    def repair_heap_upwards(self, i):
        """ Repair heap upwards, starting from the given position. """
        while i > 1:
            pi = i // 2
            if self.items[i].key < self.items[pi].key:
                self.swap_items(i, pi)
            else:
                break
            i = pi

    def repair_heap_downwards(self, i):
        """
        Repairs the heap property at node i against its children as described
        in the lecture
        """
        it = self.items
        while 2 * i < len(it):
            # First find out, which child is smaller.
            ci = 2 * i
            if ci + 1 < len(it) and it[ci + 1].key < it[ci].key:
                ci = 2 * i + 1
            # If smaller child is smaller than i swap and continue.
            if it[ci].key < it[i].key:
                self.swap_items(i, ci)
            i = ci

    def swap_items(self, i, j):
        """ Swap the items at the given positions. """
        self.items[i], self.items[j] = self.items[j], self.items[i]
        self.items[i].heap_index = i
        self.items[j].heap_index = j


class PriorityQueueItem:
    """ An item of a priority queue, with key and value. """

    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.heap_index = None

    def __repr__(self):
        """
        A priority queue item in human-readable form. For example, show element
        with key 1 and value X at position 5 as 1X@5.
        """
        return "%d%s@%d" % (self.key, self.value, self.heap_index)