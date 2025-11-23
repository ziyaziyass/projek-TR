#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE_NAME "mahasiswa.txt"
#define FILE_JURUSAN_NAME "jurusan.txt"

struct Mahasiswa {
    char nama[50];
    char nim[10];
    char jenis_kelamin;
    char alamat[50];
    char nama_jurusan[26];
};

struct Jurusan {
    char kode_jurusan[3];
    char nama_jurusan[26];
};

// === Validasi Huruf ===
int validHuruf(char teks[]) {
    for (int i = 0; teks[i]; i++) {
        char c = teks[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' '))
            return 0;
    }
    return 1;
}

// === Input Aman Angka ===
int inputAngka(const char *label) {
    int val;
    while (1) {
        printf("%s", label);
        if (scanf("%d", &val) == 1) {
            while (getchar() != '\n');
            return val;
        } else {
            printf("Input salah! Harus angka.\n");
            while (getchar() != '\n');
        }
    }
}

// === Input Aman Teks ===
void inputTeks(const char *label, char *tujuan) {
    do {
        printf("%s", label);
        scanf(" %[^\n]", tujuan);
        if (!validHuruf(tujuan))
            printf("Input hanya boleh huruf dan spasi!\n");
    } while (!validHuruf(tujuan));
}

char* cariJurusan(struct Jurusan daftar[], const char *nim){
    static char hasil[26];
    char kode_jurusan_mhs[3];
    strncpy(kode_jurusan_mhs,nim+0, 2);
    kode_jurusan_mhs[2] = '\0';
    for(int i=0;i<10;i++){
        if(strcmp(daftar[i].kode_jurusan, kode_jurusan_mhs)==0){
            return daftar[i].nama_jurusan;
        }
    }
    return "";
}

// === Membaca Jurusan.txt return Jurusan[] ===
struct Jurusan* readJurusanFile(){
    FILE *fp = fopen(FILE_JURUSAN_NAME, "r");
    struct Jurusan j;
    static struct Jurusan daftar[10];

    if(fp == NULL){
        printf("Belum ada data jurusan tersimpan! \n");
        return NULL;
    }

    int num = 0;
    while (fscanf(fp, " %2[^;];%25[^\n]",
                  j.kode_jurusan, j.nama_jurusan) == 2){
        daftar[num++] = j;
    }
    fclose(fp);
    if(num != 0){
        daftar[num].nama_jurusan[0] = '\0';
        return daftar;
    }else{
        printf("Belum ada data jurusan tersimpan!\n");
        return NULL;
    }
};

// === Membaca Mahasiswa.txt (return mahasiwa[])===
struct Mahasiswa* readMahasiswaFile(){
    FILE *fp = fopen(FILE_NAME, "r");
    struct Mahasiswa m;
    static struct Mahasiswa daftar[100];
    struct Jurusan *daftarJurusan;

    if(fp == NULL){
        printf("Belum ada data mahasiswa tersimpan! \n");
        return NULL;
    }

    int num = 0;
    while (fscanf(fp, " %9[^;];%49[^;];%c;%49[^\n]",
                  m.nim, m.nama, &m.jenis_kelamin, m.alamat) == 4){
        strcpy(m.nama_jurusan, "");
        daftar[num++] = m;
    }
    fclose(fp);

    daftarJurusan = readJurusanFile();
    if(daftarJurusan != NULL){
        for(int i=0;i<num;i++){
            strcpy(daftar[i].nama_jurusan, cariJurusan(daftarJurusan, daftar[i].nim));
        }
    }
    if(num != 0){
        daftar[num].nama[0] = '\0';
        return daftar;
    }else{
        printf("Belum ada data mahasiswa tersimpan!\n");
        return NULL;
    }
}

void tampilkanMenu() {
    system("cls");
    printf("\n=== Menu Manajemen Data ===\n");
    printf("1. Master Data Mahasiswa\n");
    printf("2. Master Data Jurusan\n");
    printf("3. Reporting\n");
    printf("4. Keluar\n");
    printf("Pilih opsi (1-4): ");
}

void tampilkanMenuMahasiswa() {
    system("cls");
    printf("\n=== Master Data Mahasiswa ===\n");
    printf("1. Daftar Mahasiswa\n");
    printf("2. Tambah Mahasiswa\n");
    printf("3. Hapus Mahasiswa\n");
    printf("4. Edit Mahasiswa\n");
    printf("5. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}

void tampilkanMenuJurusan() {
    system("cls");
    printf("\n=== Master Data Jurusan ===\n");
    printf("1. Daftar Jurusan\n");
    printf("2. Tambah Jurusan\n");
    printf("3. Edit Jurusan\n");
    printf("4. Hapus Jurusan\n");
    printf("5. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}

void tampilkanMenuReporting() {
    system("cls");
    printf("\n=== Reporting ===\n");
    printf("1. Statistik\n");
    printf("2. Unduh Data Mahasiswa\n");
    printf("3. Unduh Data Jurusan\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}
void unduhDataMahasiswa() {
    int pilihanunduhmahasiswa;
    FILE *src;
    char line[200];

    src = fopen("mahasiswa.txt", "r"); 
    if (!src) {
        printf("File sumber tidak ditemukan!\n");
        return;
    }

    printf("Pilih format unduh:\n");
    printf("1. TXT\n");
    printf("2. CSV\n");
    pilihanunduhmahasiswa = inputAngka("Masukkan pilihan (1/2): ");

    if (pilihanunduhmahasiswa == 1) {
        FILE *dst = fopen("hasil_download.txt", "w");
        if (!dst) {
            printf("Gagal membuat file TXT!\n");
            fclose(src);
            return;
        }

        fprintf(dst, "+-----------+--------------------+-----+-------------+\n");
        fprintf(dst, "|   NIM     |       Nama         | JK  |   Asal      |\n");
        fprintf(dst, "+-----------+--------------------+-----+-------------+\n");

        while (fgets(line, sizeof(line), src)) {
            char *nim = strtok(line, ";");
            char *nama = strtok(NULL, ";");
            char *jk   = strtok(NULL, ";");
            char *asal = strtok(NULL, ";\n");

            fprintf(dst, "| %-9s | %-18s | %-3s | %-11s |\n",
                    nim, nama, jk, asal);
        }

        fprintf(dst, "+-----------+--------------------+-----+-------------+\n");
        fclose(dst);
        printf("Data berhasil diunduh di: hasil_mahasiswa.txt\n");

    } else if (pilihanunduhmahasiswa == 2) {
        FILE *dst = fopen("mahasiswa.csv", "w");
        if (!dst) {
            printf("Gagal membuat file CSV!\n");
            fclose(src);
            return;
        }

        fprintf(dst, "NIM;Nama;JK;Asal\n");

        while (fgets(line, sizeof(line), src)) {
            char nim[20], nama[50], jk[5], asal[50];
            if (sscanf(line, "%[^;];%[^;];%[^;];%[^\n]", nim, nama, jk, asal) == 4) {
                fprintf(dst, "%s;%s;%s;%s\n", nim, nama, jk, asal);
            }
        }

        fclose(dst);
        printf("Data berhasil disimpan di: mahasiswa.csv\n");

    } else {
        printf("Pilihan tidak valid!\n");
    }

    fclose(src);

    printf("Tekan ENTER untuk kembali...");
    getchar();
}
void unduhDataJurusan() {
    int pilihanunduhjurusan;
    FILE *src;

    src = fopen("jurusan.txt", "r");
    if (!src) {
        printf("File jurusan.txt tidak ditemukan!\n");
        return;
    }

    printf("Pilih format unduh:\n");
    printf("1. TXT\n");
    printf("2. CSV\n");
    pilihanunduhjurusan = inputAngka("Masukkan pilihan (1/2): ");

    char line[200];

    if (pilihanunduhjurusan == 1) {
        FILE *dst = fopen("hasil_jurusan.txt", "w");
        if (!dst) { fclose(src); return; }

        fprintf(dst, "+------------+----------------------+\n");
        fprintf(dst, "|   Kode     |     Nama Jurusan     |\n");
        fprintf(dst, "+------------+----------------------+\n");

        while (fgets(line, sizeof(line), src)) {
            char *kode = strtok(line, ";");
            char *nama = strtok(NULL, "\n");
            fprintf(dst, "| %-10s | %-20s |\n", kode, nama);
        }

        fprintf(dst, "+------------+----------------------+\n");
        fclose(dst);
        printf("Data jurusan berhasil diunduh ke: hasil_jurusan.txt\n");

    } else if (pilihanunduhjurusan == 2) {
        FILE *dst = fopen("jurusan.csv", "w");
        if (!dst) { fclose(src); return; }

        fprintf(dst, "Kode;Nama Jurusan\n");

        while (fgets(line, sizeof(line), src)) {
            char *kode = strtok(line, ";");
            char *nama = strtok(NULL, "\n");
            fprintf(dst, "%s;%s\n", kode, nama);
        }

        fclose(dst);
        printf("Data jurusan berhasil diunduh ke: jurusan.csv\n");

    } else {
        printf("Pilihan tidak valid!\n");
    }

    fclose(src);
}
#include <stdio.h>
#include <string.h>

void statistikJurusan() {
    FILE *src = fopen("mahasiswa.txt", "r");
    if (!src) {
        printf("File mahasiswa.txt tidak ditemukan!\n");
        return;
    }

    char line[200], nim[20], nama[50], jk[5], asal[50];
    int countTI = 0, countSI = 0, countDKV = 0, countPTI = 0;

    while (fgets(line, sizeof(line), src)) {
        if (sscanf(line, "%[^;];%[^;];%[^;];%[^\n]", nim, nama, jk, asal) == 4) {
            // ambil dua digit awal NIM
            char kode[3];
            strncpy(kode, nim, 2);
            kode[2] = '\0';

            // Hitung berdasarkan mapping
            if (strcmp(kode, "67") == 0) countTI++;
            else if (strcmp(kode, "68") == 0) countSI++;
            else if (strcmp(kode, "69") == 0) countDKV++;
            else if (strcmp(kode, "70") == 0) countPTI++;
        }
    }

    fclose(src);

    // Tampilkan tabel
    printf("+----------------------+--------+\n");
    printf("| Nama Jurusan         | Jumlah |\n");
    printf("+----------------------+--------+\n");
    printf("| Teknik Informatika   | %-6d |\n", countTI);
    printf("| Sistem Informasi     | %-6d |\n", countSI);
    printf("| DKV                  | %-6d |\n", countDKV);
    printf("| Pendidikan TI        | %-6d |\n", countPTI);
    printf("+----------------------+--------+\n\n");
    printf("Tekan ENTER untuk kembali...");
    getchar();
}


bool cariMahasiswaByNim(struct Mahasiswa daftar[], const char *nim) {
    for (int i = 0; i < 100; i++) {
        if (daftar[i].nim[0] == '\0')
            continue;

        if (strcmp(daftar[i].nim, nim) == 0) {
            return true;
        }
    }
    return false;
}

int cariIndexMahasiswaByNim(struct Mahasiswa daftar[], const char *nim) {
    for (int i = 0; i < 100; i++) {
        if (daftar[i].nim[0] == '\0')
            continue;

        if (strcmp(daftar[i].nim, nim) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    int pilihanUtama, pilihanMahasiswa, pilihanJurusan, pilihanreporting;

    struct Mahasiswa *data;
    int jumlahMahasiswa = 0;

    struct Jurusan *dataJurusan;
    int jumlahJurusan = 0;

    do {
        tampilkanMenu();
        pilihanUtama = inputAngka("");
        switch (pilihanUtama) {
                case 1: // MASTER DATA MAHASISWA
                do {
                    tampilkanMenuMahasiswa();
                    pilihanMahasiswa = inputAngka("");

                    switch (pilihanMahasiswa) {
                        case 1: // Daftar Mahasiswa
                            system("cls");
                            printf("=========================================================================================================\n");
                            printf("|\t\t\t\t\t\tDaftar Mahasiswa\t\t\t\t\t|\n");
                            printf("=========================================================================================================\n");
                            data = readMahasiswaFile();
                            if (data == NULL) {
                                printf("Daftar mahasiswa kosong atau file tidak ditemukan.\n");
                                break;
                            }
                            if (data[0].nim == NULL){
                                printf("Daftar mahasiswa kosong.\n");
                            }else{
                                printf("| %-3s | %-10s | %-20s | %-20s | %-13s | %-20s |\n",
                                       "No", "NIM", "Jurusan", "Nama", "Jenis Kelamin", "Alamat");
                                printf("---------------------------------------------------------------------------------------------------------\n");
                                for (int i = 0; i < 100; i++) {
                                    if (data[i].nama[0] == '\0') {
                                        break;
                                    } else {
                                        printf("| %-3d | %-10s | %-20s | %-20s | %-13c | %-20s |\n",
                                               i + 1,
                                               data[i].nim,
                                               data[i].nama_jurusan,
                                               data[i].nama,
                                               data[i].jenis_kelamin,
                                               data[i].alamat);
                                    }
                                }
                            }
                            system("pause");
                            break;


                        case 2: // Tambah Mahasiswa
                            {
                                system("cls");
                                int pilihan_input_jurusan;
                                int input_nim;
                                struct Mahasiswa mhs;
                                printf("=== Tambah Data Mahasiswa ===\n");
                                printf("Pilihan jurusan : \n");
                                dataJurusan = readJurusanFile();
                                data = readMahasiswaFile();
                                if (dataJurusan == NULL) {
                                    printf("Daftar jurusan kosong atau file tidak ditemukan.\n");
                                    break;
                                }
                                if (dataJurusan[0].kode_jurusan == NULL){
                                    printf("Daftar Jurusan kosong.\n");
                                }else{
                                    for (int i = 0; i < 100; i++) {
                                        if (dataJurusan[i].kode_jurusan[0] != '\0') {
                                            printf("%d. (%s) %s\n",
                                                   i + 1,
                                                   dataJurusan[i].kode_jurusan,
                                                   dataJurusan[i].nama_jurusan);
                                        } else {
                                            break;
                                        }
                                    }
                                }

                                char nim_str[10];
                                char nim_cari[10];
                                bool hasil_cek = false;

                                printf("Pilih jurusan : ");
                                pilihan_input_jurusan = inputAngka("");
                                do{
                                    printf("Masukkan NIM : %s", dataJurusan[pilihan_input_jurusan-1].kode_jurusan);
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    // gabungkan kode jurusan + nim ke nim_cari (tanpa mengubah kode_jurusan)
                                    sprintf(nim_cari, "%s%d", dataJurusan[pilihan_input_jurusan-1].kode_jurusan, input_nim);
                                    hasil_cek = cariMahasiswaByNim(data, nim_cari);

                                    if(hasil_cek==1){
                                        printf("Maaf NIM sudah terpakai, input ulang NIM! \n");
                                    }

                                }while(hasil_cek == 1);

                                // tambahkan nim dengan kode jurusan
                                strcpy(mhs.nim, (strcat(dataJurusan[pilihan_input_jurusan-1].kode_jurusan, nim_str)));

                                // input data nama dst
                                inputTeks("Masukkan nama : ", mhs.nama);
                                printf("Masukkan jenis kelamin : ");
                                scanf(" %c",&mhs.jenis_kelamin);
                                inputTeks("Masukkan alamat: ", mhs.alamat);

                                //append ke mahasiswa.txt

                                FILE *fp = fopen(FILE_NAME, "a");
                                fprintf(fp, "%s;%s;%c;%s\n", mhs.nim, mhs.nama, mhs.jenis_kelamin, mhs.alamat);
                                fclose(fp);

                                printf("\n\nData berhasil ditambahkan!\n");
                                system("pause");
                                break;
                            }
                        case 3:
                            {
                                // Hapus Mahasiswa
                                /*
                                   1. Baca file data mahasiswa
                                   2. Cari mahasiswa yg ingin dihapus
                                   3. Simpan index/urutan mahasiswa yg ingin dihapus
                                   4. Tulis ulang file tanpa urutan mahasiswa yg dihapus
                                */
                                system("cls");
                                printf("=== Hapus Data Mahasiswa ===\n");
                                data = readMahasiswaFile();
                                int input_nim;
                                char nim_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan NIM : ");
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    if(input_nim == 0){
                                        break;
                                    }
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexMahasiswaByNim(data, nim_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan/nim tidak ditemukan
                                        printf("Maaf NIM tidak ditemukan, input ulang NIM (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        // hasil_cek = index mahasiswa yg ingin dihapus
                                        printf("Data Mahasiswa : \n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        printf("Nama          : %s\n", data[hasil_cek].nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : %c\n", data[hasil_cek].jenis_kelamin);
                                        printf("Alamat        : %s\n\n", data[hasil_cek].alamat);
                                        printf("Yakin ingin menghapus data mahasiswa? (y/n)");
                                        char pilihan;
                                        scanf("%c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (data[i].nim[0] == '\0') {
                                                    break;
                                                }

                                                // Skip data yang akan dihapus (hasil_cek = index yang ditemukan)
                                                if (i == hasil_cek) {
                                                    continue;
                                                }else{
                                                    // Tulis data lain ke file
                                                    fprintf(fp, "%s;%s;%c;%s\n",
                                                            data[i].nim,
                                                            data[i].nama,
                                                            data[i].jenis_kelamin,
                                                            data[i].alamat);
                                                }

                                            }
                                            fclose(fp);
                                            printf("Data Mahasiswa berhasil dihapus! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;

                                        }
                                    }
                                }while(hasil_cek == -1);

                                break;
                            }

                        case 4:
                                // Edit Mahasiswa
                                /*
                                   1. Baca file data mahasiswa
                                   2. Cari mahasiswa yg ingin diedit
                                   3. Simpan index/urutan mahasiswa yg ingin diedit
                                   4. Input data baru (NIM tidak boleh diedit)
                                   5. Tulis ulang file tetapi index yg ingin diedit, tulis data yg baru
                                */
                                system("cls");
                                printf("=== Edit Data Mahasiswa ===\n");
                                data = readMahasiswaFile();
                                int input_nim;
                                char nim_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan NIM : ");
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    if(input_nim == 0){
                                        break;
                                    }
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexMahasiswaByNim(data, nim_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan/nim tidak ditemukan
                                        printf("Maaf NIM tidak ditemukan, input ulang NIM (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        char input_nama[49];
                                        char input_alamat[49];
                                        // hasil_cek = index mahasiswa yg ingin diubah
                                        printf("Data Mahasiswa Sebelumnya: \n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        printf("Nama          : %s\n", data[hasil_cek].nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : %c\n", data[hasil_cek].jenis_kelamin);
                                        printf("Alamat        : %s\n\n", data[hasil_cek].alamat);
                                        printf("-----------------------------------------------\n\n");
                                        printf("Perubahan Data Mahasiswa :\n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        inputTeks("Nama          : ", input_nama);
                                        strcpy(data[hasil_cek].nama, input_nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : ");
                                        scanf(" %c",&data[hasil_cek].jenis_kelamin);
                                        inputTeks("Alamat        : ", input_alamat);
                                        strcpy(data[hasil_cek].alamat, input_alamat);
                                        printf("\nYakin ingin mengubah data mahasiswa? (y/n)");
                                        char pilihan;
                                        scanf(" %c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (data[i].nim[0] == '\0') {
                                                    break;
                                                }

                                                // Tulis ulang ke file
                                                fprintf(fp, "%s;%s;%c;%s\n",
                                                            data[i].nim,
                                                            data[i].nama,
                                                            data[i].jenis_kelamin,
                                                            data[i].alamat);


                                            }
                                            fclose(fp);
                                            printf("Data Mahasiswa berhasil diubah! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;
                                        }
                                    }
                                }while(hasil_cek == -1);
                            break;

                        case 5:
                            printf("Kembali ke menu utama...\n");
                            break;
                        default:
                            printf("Opsi tidak valid.\n");
                            break;
                    }
                } while (pilihanMahasiswa != 5);
                break;

            case 2: // MASTER DATA JURUSAN
                do {
                    tampilkanMenuJurusan();
                    pilihanJurusan = inputAngka("");

                    switch (pilihanJurusan) {
                          case 1: // Daftar Jurusan
                            system("cls");
                            printf("============================================\n");
                            printf("|              Daftar Jurusan              |\n");
                            printf("============================================\n");
                            dataJurusan = readJurusanFile();
                            if (dataJurusan == NULL) {
                                printf("Daftar jurusan kosong atau file tidak ditemukan.\n");
                                break;
                            }
                            if (dataJurusan[0].kode_jurusan == NULL){
                                printf("Daftar Jurusan kosong.\n");
                            }else{
                                printf("| %-3s | %-10s | %-19s |\n",
                                       "No", "Kode Jurusan", "Nama Jurusan");
                                printf("--------------------------------------------\n");
                                for (int i = 0; i < 100; i++) {
                                    if (dataJurusan[i].kode_jurusan[0] != '\0') {
                                        printf("| %-3d | %-12s | %-19s |\n",
                                               i + 1,
                                               dataJurusan[i].kode_jurusan,
                                               dataJurusan[i].nama_jurusan);
                                    } else {
                                        break;
                                    }
                                }
                                printf("--------------------------------------------\n");
                            }
                            system("pause");
                            break;


                        case 2: // Tambah Jurusan
                            /*
                             inputTeks("Masukkan Kode Jurusan: ", jurusan[jumlahJurusan].kode);
                            inputTeks("Masukkan Nama Jurusan: ", jurusan[jumlahJurusan].nama);
                            jumlahJurusan++;
                            printf("Data jurusan berhasil ditambahkan!\n");
                            break;



                            */
                            break;

                        case 3: // Edit Jurusan

                           /* if (jumlahJurusan==0) { printf("Data kosong.\n"); break; }
                            char kodeEdit[10];
                            inputTeks("Masukkan Kode Jurusan yang akan diedit: ", kodeEdit);
                            int idxJur=-1;
                            for (int i=0;i<jumlahJurusan;i++)
                                if (strcmp(jurusan[i].kode,kodeEdit)==0) { idxJur=i; break; }
                            if (idxJur==-1) printf("Jurusan tidak ditemukan.\n");
                            else {
                                inputTeks("Masukkan Kode baru: ", jurusan[idxJur].kode);
                                inputTeks("Masukkan Nama baru: ", jurusan[idxJur].nama);
                                printf("Data berhasil diupdate!\n");
                            }*/

                            break;

                        case 4: // Hapus Jurusan

                           /* if (jumlahJurusan==0) { printf("Data kosong.\n"); break; }
                            char kodeHapus[10];
                            inputTeks("Masukkan Kode Jurusan yang akan dihapus: ", kodeHapus);
                            int idxHapus=-1;
                            for (int i=0;i<jumlahJurusan;i++)
                                if (strcmp(jurusan[i].kode,kodeHapus)==0) { idxHapus=i; break; }
                            if (idxHapus==-1) printf("Jurusan tidak ditemukan.\n");
                            else {
                                char konfirmasi;
                                printf("Hapus jurusan %s (Kode %s)? (Y/N): ", jurusan[idxHapus].nama, kodeHapus);
                                scanf(" %c",&konfirmasi);
                                if (konfirmasi=='Y'||konfirmasi=='y') {
                                    for (int i=idxHapus;i<jumlahJurusan-1;i++)
                                        jurusan[i]=jurusan[i+1];
                                    jumlahJurusan--;
                                    printf("Data berhasil dihapus.\n");
                                } else printf("Penghapusan dibatalkan.\n");
                            }*/

                            break;

                        case 5:
                            printf("Kembali ke menu utama...\n");
                            break;

                        default:
                            printf("Opsi tidak valid.\n");
                            break;
                    }

                } while (pilihanJurusan != 5);
                break;

            case 3://REPORTING
            do {
                    tampilkanMenuReporting();
                    pilihanreporting = inputAngka("");

                    switch (pilihanreporting) {
                        case 1:
                        system("cls");
                        statistikJurusan();
                            break;
                        case 2:
                        system("cls");
                        unduhDataMahasiswa();
                        break;
                        case 3:
                        system("cls");
                        unduhDataJurusan();
                        break;
                        case 4:
                            printf("Kembali ke menu utama...\n");
                            break;
                    default:
                        printf("Opsi tidak valid.\n");
                        break;
                    }

                } while (pilihanreporting != 4);
                break;

            case 4:
                printf("Keluar dari program...\n");
                break;

            default:
                printf("Opsi tidak valid.\n2");
                break;
        }
    }while(pilihanUtama != 4);

    return 0;
}
