#include <iostream>
#include <string>
#include <fstream>
#include <limits>


using namespace std;


struct Karyawan{
	int pkey;
	string nama;
	string npk;
	string bagian;
	
};

int getOption();
void cekDatabase(fstream &data);
void writeData(fstream &data, int posisi, Karyawan &inputKaryawan);
int getData(fstream &data);
Karyawan readData(fstream &data, int posisi);
void tambahKaryawan(fstream &data);
void tampilKaryawan(fstream &data);
void updateKaryawan(fstream &data);
void hapusKaryawan(fstream &data);


int main(){
	fstream data;
	cekDatabase(data);

	int pilihan = getOption();
	char lanjut;

	enum option {CREATE = 1, READ, UPDATE, DELETE, FINISH};
	while(pilihan != FINISH){
		switch(pilihan) {
			case CREATE:
				cout << "|-- Tambah data --|" << endl;
				tambahKaryawan(data);
				break;
			case READ:
				cout << "|-- Tampilkan data --|" << endl;
				tampilKaryawan(data);
				break;
			case UPDATE:
				cout << "|-- Ubah data --|" << endl;
				tampilKaryawan(data);
				updateKaryawan(data);
				tampilKaryawan(data);
				break;
			case DELETE:
				cout << "|-- Hapus data --|" << endl;
				tampilKaryawan(data);
				hapusKaryawan(data);
				tampilKaryawan(data);
				break;
			default:
				cout << "Pilihan tidak di temukan !!" << endl;
				cout << "Masukan 1 - 5 untuk memilih..!!" << endl;
				break;
		}
		label_lanjutkan:
		cout << "Lanjutkan? (y/n)> ";
		cin >> lanjut;
		if ((lanjut == 'y') | (lanjut == 'Y')){
			pilihan = getOption();
			
		}
			else if ((lanjut == 'n') | (lanjut == 'N')){
				break;
			}
			else {
				cout << "Masukan Y / N..!!!" << endl;
				goto label_lanjutkan;
			}
		}
		
	cout << "Akhir program" << endl;
}



void writeData(fstream &data, int posisi, Karyawan &inputKaryawan){
	data.seekp((posisi - 1)*sizeof(Karyawan), ios::beg);
	data.write(reinterpret_cast<char*>(&inputKaryawan), sizeof(Karyawan));
}

Karyawan readData(fstream &data, int posisi){
	Karyawan readKaryawan;
	data.seekg((posisi - 1)*sizeof(Karyawan), ios::beg);
	data.read(reinterpret_cast<char*>(&readKaryawan), sizeof(Karyawan));
	return readKaryawan;
}

void tampilKaryawan(fstream &data){
	int size;
	Karyawan lihatKaryawan;

	size = getData(data);
	cout << "No\t|\tNama\t|\tNPK\t|\tBagian" << endl;
	for(int i = 1; i <= size; i++){
		lihatKaryawan = readData(data, i);
		cout << 1 << "\t|\t";
		cout << lihatKaryawan.nama << "\t|\t";
		cout << lihatKaryawan.npk << "\t|\t";
		cout << lihatKaryawan.bagian << endl;
	}
}

void hapusKaryawan(fstream &data){
	int no, offset, size;
	Karyawan blankKaryawan, tempKaryawan;
	fstream dataSementara;

	size = getData(data);
	cout << "Pilih nomor : ";
	cin >> no;

	writeData(data,no,blankKaryawan);


	offset = 0;
	dataSementara.open("temp.dat", ios::trunc | ios::out | ios::in | ios::binary);

	for (int i = 1; i <= size; i++){
		tempKaryawan = readData(data,i);

		if (!tempKaryawan.nama.empty()){
			writeData(dataSementara,i,tempKaryawan);
		}
		else {
			offset++;
			cout << "Data Terhapus !!" << endl;
		}
	}

	size = getData(data);
	data.close();
	data.open("data.bin", ios::trunc | ios::out | ios::binary);
	data.close();
	data.open("data.bin", ios::trunc | ios::in | ios::binary);

	for (int i = 1; i <= size; i++){
		tempKaryawan = readData(dataSementara,i);
		writeData(data,i,tempKaryawan);
	}


}

void updateKaryawan(fstream &data){
	int no;
	Karyawan updateInput;

	ubah:
	cout << "Pilih nomor > ";
	cin >> no;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	updateInput = readData(data, no);

	cout << "\n-- Data sekarang --" << endl;
	cout << "Nama : " << updateInput.nama << endl;
	cout << "NPK : " << updateInput.npk << endl;
	cout << "Bagian : " << updateInput.bagian << endl;

	cout << "\n\n-- Masukan perubahan data -- " << endl;
	cout << "Nama : ";
	getline(cin, updateInput.nama);
	cout << "NPK : ";
	getline(cin, updateInput.npk);
	cout << "Bagian : ";
	getline(cin, updateInput.bagian);

	writeData(data, no, updateInput);

}

void cekDatabase(fstream &data){
	data.open("data.bin", ios::out | ios::in | ios::binary);

	if (data.is_open()){
		cout << endl;
	}
	else {
		data.close();
		data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
	}
}

int getOption() {
	int input;
	system("cls");
	cout << "|==============================|" << endl;
	cout << "|- PROGRAM CRUD DATA KARYAWAN -|" << endl;
	cout << "|==============================|" << endl;
	cout << "| 1.) Tambah data karyawan     |" << endl;
	cout << "| 2.) Tampilkan data karyawan  |" << endl;
	cout << "| 3.) Ubah data karyawan       |" << endl;
	cout << "| 4.) Hapus data karyawan      |" << endl;
	cout << "| 5.) Selesai                  |" << endl;
	cout << "|==============================|" << endl;
	cout << "choose[1-5]> ";
	cin >> input;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	return input;
}

void tambahKaryawan(fstream &data){
	
	Karyawan inputKaryawan, lastKaryawan;
	int size;
	size = getData(data);

	if (size == 0){
		inputKaryawan.pkey = 1;
	}
	else {
		lastKaryawan = readData(data, size);
		inputKaryawan.pkey = lastKaryawan.pkey + 1;
	}

	cout << "Nama : ";
	getline(cin,inputKaryawan.nama);
	cout << "NPK : ";
	getline(cin,inputKaryawan.npk);
	cout << "Bagian : ";
	getline(cin,inputKaryawan.bagian);

	writeData(data, size+1, inputKaryawan);
}

int getData(fstream &data){
	int start, end;
	data.seekg(0, ios::beg);
	start = data.tellg();
	data.seekg(0, ios::end);
	end = data.tellg();
	return (end - start) / sizeof(Karyawan);
}

