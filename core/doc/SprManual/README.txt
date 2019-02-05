texlive2018でpdf版のmakeが失敗する件

texlive2018の配布版をインストールしたままでは、SprManualのpdf版もmakeに失敗する。

原因：
　　インストールしたままの状態ではiccprofileの参照に失敗するため。

対処：
　　gsの起動パラメータに、iccprofileが置かれているディレクトリ
　　　　C:\texlive\2018\tlpkg\tlgs\iccprofiles\
　　を追加する。

修正：
　　C:/texlive/2018/texmf-dist/dvipdfmx/dvipdfmx.cfg 159行目を

　　D "rungs -q -dNOPAUSE -dBATCH -dEPSCrop -sPAPERSIZE=a0 -sDEVICE=pdfwrite -dCompatibilityLevel=%v \
　　　-dAutoFilterGrayImages=false -dGrayImageFilter=/FlateEncode -dAutoFilterColorImages=false \
　　　-dColorImageFilter=/FlateEncode -dAutoRotatePages=/None \
　　　-sICCProfilesDir=c:/texlive/2018/tlpkg/tlgs/iccprofiles/ \
　　　-sOutputFile='%o' '%i' -c quit"

　　とする（-sICCProfilesDir=...を追加。実際には1行で記述する）。
