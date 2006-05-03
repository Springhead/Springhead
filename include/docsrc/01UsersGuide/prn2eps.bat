PATH=%PATH%;C:\ProgFile\Ghostgum\gsview
for %%i in (*.prn) do epstool -b -c -o%%~ni.eps %%i
for %%i in (*.eps) do gswin32c -r48 -dEPSCrop -dNOPAUSE -dBATCH  -dQUIET -sDEVICE=jpeg -sOutputFile="%%~ni.jpg" %%i
