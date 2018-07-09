byte handle;
string nameFile;
int flashValue[15];

void CreateFlash (string name, int size){
	nameFile = name;
	DeleteFile(nameFile);
	CreateFile(nameFile, size, handle);
}


void WriteFlash(int value) {
	WriteLn(handle, value);
	Wait(50);
}

void CloseFlash(){
	CloseFile(handle);
}


void ReadFlash(string name) {

	int size, input;
	int cont = 0;

	OpenFileRead(name, size, handle);

	for(int i = 0; i < SizeOf(flashValue); i++)
		flashValue[i] = 0;

	until( ReadLn(handle,input) != NO_ERR ) {
		flashValue[cont] = input;
		cont++;
		Wait(10);
	}
	CloseFile(handle);

}