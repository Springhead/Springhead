set PATH=..\..\bin;..\..\bin\swig
swig -c++ -w312,319,325,362,401,402 -python -static_link BasePy.i
copy Base.py ..\..\bin\src\Python\Lib
pause
