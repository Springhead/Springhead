## ダウンロード

Springhead Library は GitHub で管理されており、
次の URL からダウンロードすることができます。
**以下、ダウンロードするディレクトリを "C:/Springhead" として説明を進めます。**

```
> chdir C:/Springhead
> git clone --recurse-submodules https://github.com/sprphys/Springhead
```

デフォルトで python が利用できる環境であれば
サブモジュール buildtool は必要ありません。
また、外部パッケージ boost, glew, glut, glui を別途インストールして使用するのであれば
サブモジュール dependency は必要ありません。
そのような場合には、次のようにしてサブモジュールを選択してください。
```
> chdir C:/Springhead
> git clone https://github.com/sprphys/Springhead
> git submodule update --init --checkout buildtool
> git submodule update --init --checkout dependency
```

Windows explorer ならば右クリックで GUI が利用できます。
Git clone の画面で recursive にチェックを入れると
サブモジュールも一緒にダウンロードできます。

<img src="/fig/SpringheadClone1.jpg" width="600px">
![Springhead Download 2](/fig/SpringheadClone2.jpg)

> サブモジュールを導入するのに必要なディスク容量は、
buildtool が約 32MB、dependency が約 550MB です。

