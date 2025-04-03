# Meminta input kalimat dari pengguna
kalimat = input("Masukkan kalimat: ")

# Memecah kalimat menjadi kata-kata
kata_list = kalimat.split()

# Menghitung frekuensi kata
frekuensi_kata = {}

for kata in kata_list:
    if kata in frekuensi_kata:
        frekuensi_kata[kata] += 1
    else:
        frekuensi_kata[kata] = 1

print("Frekuensi kata dalam kalimat:", frekuensi_kata)