class Node:
    def __init__(self, data=None, next=None):
        self.data = data
        self.next = next

class LinkedList:
    def __init__(self):
        self.head = None

    def insert_at_beginning(self, data):
        node = Node(data, self.head)
        self.head = node

    def print(self):
        if self.head is None:
            print("Linked List is empty")
            return

        itr = self.head
        while itr:
            next_address = id(itr.next) if itr.next else None
            print(f"Data: {itr.data}, Address: {id(itr)}, Next Address: {next_address}")
            itr = itr.next

    def inserting_at_end(self, data):
        if self.head is None:
            self.head = Node(data, None)
            return

        itr = self.head
        while itr.next:
            itr = itr.next

        itr.next = Node(data, None)

if __name__ == '__main__':
    ll = LinkedList()
    ll.insert_at_beginning(8)
    ll.inserting_at_end(16)
    ll.insert_at_beginning(2)
    ll.print()
