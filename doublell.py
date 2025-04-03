class Node:
    def __init__(self, data):
        self.data = data
        self.prev = None  # Pointer ke node sebelumnya
        self.next = None  # Pointer ke node berikutnya

class DoubleLinkedList:
    def __init__(self):
        self.head = None  # Head dari DLL

    # 1. Menambahkan node di awal
    def insert_at_beginning(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node

    # 2. Menambahkan node di akhir
    def insert_at_end(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
        else:
            temp = self.head
            while temp.next:
                temp = temp.next
            temp.next = new_node
            new_node.prev = temp

    # 3. Menghapus node berdasarkan nilai
    def delete_node(self, key):
        temp = self.head
        while temp and temp.data != key:
            temp = temp.next
        if temp is None:
            print(f"Node dengan data {key} tidak ditemukan.")
            return
        if temp.prev:
            temp.prev.next = temp.next
        if temp.next:
            temp.next.prev = temp.prev
        if temp == self.head:  # Jika node yang dihapus adalah head
            self.head = temp.next
        del temp  # Hapus dari memori

    # 4. Menampilkan list dari awal ke akhir
    def display_forward(self):
        temp = self.head
        while temp:
            print(temp.data, end=" <-> ")
            temp = temp.next
        print("None")

    # 5. Menampilkan list dari akhir ke awal (reverse traversal)
    def display_backward(self):
        temp = self.head
        if temp is None:
            print("List kosong")
            return
        while temp.next:  # Pergi ke node terakhir
            temp = temp.next
        while temp:  # Traversing mundur
            print(temp.data, end=" <-> ")
            temp = temp.prev
        print("None")

# Contoh Penggunaan
dll = DoubleLinkedList()
dll.insert_at_beginning(10)
dll.insert_at_end(20)
dll.insert_at_end(30)
dll.insert_at_beginning(5)

print("Traversal maju:")
dll.display_forward()  # Output: 5 <-> 10 <-> 20 <-> 30 <-> None

print("Traversal mundur:")
dll.display_backward()  # Output: 30 <-> 20 <-> 10 <-> 5 <-> None

dll.delete_node(20)
print("Setelah menghapus 20:")
dll.display_forward()  # Output: 5 <-> 10 <-> 30 <-> None