rem Project以下をVSSから取得します

rem このファイルはすでに使われていないように見えます
rem SVNレポジトリにはSpringhead以下しか入れていないのであっても意味ないです

cd ..\..
ss Workfold $/Project .
ss Get $/Project -R > Springhead\test\log\GetVSS.log
cd Springhead\test
