arr = [1,2,3,4,5,6,7,8,9,10,11,12,11,13,14,15,16,17,18,19,20]

ganjil = []
genap = []
for i in arr:
    if i % 2 == 0:
        genap.append(i)
    else:
        ganjil.append(i)

print("Bilangan Genap: ", genap)
print("Bilangan Ganjil: ", ganjil)