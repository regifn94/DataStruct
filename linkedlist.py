# Definisi class Node untuk merepresentasikan setiap elemen dalam linked list
class Node:
    def __init__(self, data):
        self.data = data  # Menyimpan data
        self.next = None  # Referensi ke node berikutnya, awalnya None

# Definisi class LinkedList untuk mengelola seluruh linked list
class LinkedList:
    def __init__(self):
        self.head = None  # Head (kepala) dari linked list, awalnya None

    # Method untuk menambahkan elemen di akhir linked list
    def append(self, data):
        new_node = Node(data)  # Membuat node baru
        if self.head is None:  # Jika linked list kosong
            self.head = new_node
            return
        last_node = self.head
        while last_node.next:  # Melintasi sampai node terakhir
            last_node = last_node.next
        last_node.next = new_node  # Menambahkan node baru di akhir

    # Method untuk menampilkan seluruh elemen dalam linked list
    def print_list(self):
        current_node = self.head
        while current_node:  # Melintasi setiap node
            print(current_node.data, end=" -> ")
            current_node = current_node.next
        print("None")  # Menandakan akhir dari linked list

    # Method untuk menambahkan elemen di awal linked list
    def prepend(self, data):
        new_node = Node(data)  # Membuat node baru
        new_node.next = self.head  # Node baru menunjuk ke head saat ini
        self.head = new_node  # Head baru adalah node baru

    # Method untuk menghapus node dengan data tertentu
    def delete_with_value(self, data):
        if self.head is None:  # Jika linked list kosong
            return

        # Jika node yang akan dihapus adalah head
        if self.head.data == data:
            self.head = self.head.next
            return

        current_node = self.head
        while current_node.next:  # Melintasi linked list
            if current_node.next.data == data:
                current_node.next = current_node.next.next  # Melewati node yang dihapus
                return
            current_node = current_node.next

# Contoh penggunaan
llist = LinkedList()
llist.append(10)  # Menambahkan 10 ke linked list
llist.append(20)  # Menambahkan 20 ke linked list
llist.append(30)  # Menambahkan 30 ke linked list

llist.prepend(5)  # Menambahkan 5 di awal linked list

llist.print_list()  # Output: 5 -> 10 -> 20 -> 30 -> None

llist.delete_with_value(20)  # Menghapus node dengan data 20
llist.print_list()  # Output: 5 -> 10 -> 30 -> None