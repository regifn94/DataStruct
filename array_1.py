arrays = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
 # todo mencari nilai terbesar 
 
 # 1. deklarasi variable untuk menampung nnilai terbesar
maxxx = arrays[0]

# 2. Looping arrays
for array in arrays:
    # 3. check nilai terbesar
    if array > maxxx:
        #4. set nilai terbesar
        maxxx = array

# 5. print nilai terbesar
print("nilai terbesar ", maxxx)


# List untuk bilangan genap dan ganjil
genap = []
ganjil = []

# Memisahkan bilangan genap dan ganjil
for num in arrays:
    if num % 2 == 0:
        genap.append(num)
    else:
        ganjil.append(num)

# Menampilkan hasil
print("Bilangan Genap:", genap)
print("Bilangan Ganjil:", ganjil)