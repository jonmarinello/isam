FOR %%i IN (*.C) DO MSC %%i/AS;
FOR %%i IN (*.OBJ) DO LIB ..\LIB\ISMAINTS -+%%i;
DEL ..\LIB\ISMAINTS.BAK
DEL *.OBJ
