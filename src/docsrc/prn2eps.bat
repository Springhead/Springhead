PATH=%PATH%;C:\ProgFile\Ghostgum\gsview
for %%i in (*.prn) do epstool -b -c -o%%~ni.eps %%i
for %%i in (*.eps) do gswin32c -r72 -dEPSCrop -dNOPAUSE -dBATCH  -dQUIET -sDEVICE=png256 -sOutputFile="%%~ni.png" %%i
