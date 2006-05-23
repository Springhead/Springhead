/** \page pageMoreTutoGRSdk	グラフィックスSDK チュートリアル

<br>
\section contentsMoreTutoGRSdk	◇ Contents ◇
　　 \ref moreTutoGRSdk						<br>
　　　　 \ref moreTutoCreateGRSdk			<br>
　　 \ref moreTutoGRRender					<br>
　　　　 \ref moreTutoCreateGRRender		<br>
　　 \ref moreTutoGRDevice					<br>
　　　　 \ref moreTutoCreateGRDevice		<br>
　　　　 \ref moreTutoInitGRDevice			<br>
　　　　 \ref moreTutoSetGRDevice			<br>
　　 \ref moreTutoTransMatrix				<br>
　　　　 \ref moreTutoLookAt				<br>
　　　　 \ref moreTutoMultModelMatrix		<br>
　　　　 
<br><br>
<hr>

<!-------------------- 1．　グラフィックスSDK ----------------------------------------->
\section moreTutoGRSdk		1．　グラフィックスSDK
はじめに、グラフィックスに必要となるSDKを用意します．

\subsection moreTutoCreateGRSdk		1.1．　グラフィックスSDKの作成
グラフィックスSDKを作成します．
\verbatim
    GRSdkIf* grSdk = CreateGRSdk();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoGRSdk">top of page</a>
</div> 
<hr>

<!-------------------- 2．　デバッグ情報レンダラー ------------------------------------>
\section moreTutoGRRender		2．　デバッグ情報レンダラー
グラフィックスSDKから、デバッグ情報レンダラーを作成します．

\subsection moreTutoCreateGRRender		2.1．　デバッグ情報レンダラーの作成
デバッグ情報レンダラーを作成します．
\verbatim
    GRDebugRenderIf* render = grSdk->CreateDebugRender();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoGRSdk">top of page</a>
</div> 
<hr>

<!-------------------- 3．　レンダラー用デバイス ------------------------------------>
\section moreTutoGRDevice		3．　レンダラー用デバイス
レンダリング用デバイスとして、OpenGL または Direct 3D を選択できます．
ただし現時点では Direct 3D の API が実装されていませんので描画されません．
今後対応する予定です．デバイスは、OpenGL を選択して下さい．

\subsection moreTutoCreateGRDevice		3.1．　レンダラー用デバイスの作成
OpenGLレンダラー用デバイスを作成します．
\verbatim
    GRDeviceGLIf* grDevice = grSdk->CreateDeviceGL(window);
\endverbatim
<br>

\subsection moreTutoInitGRDevice		3.2．　レンダラー用デバイスの初期化
レンダラー用デバイスの初期化を行います．<br>
ライティング、デプステスト、法線の正規化などを有効にします．
\verbatim
    grDevice->Init();
\endverbatim
<br>


\subsection moreTutoSetGRDevice		3.3．　レンダラー用デバイスの設定
レンダラー用デバイスを設定します．
\verbatim
    render->SetDevice(grDevice);	// デバイスの設定
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoGRSdk">top of page</a>
</div> 
<hr>

<!-------------------- 4．　変換行列  ------------------------------->
\section moreTutoTransMatrix		4．　変換行列
Springhead2 では、視点行列、モデル行列、射影行列を記憶する行列を内部で持っています．<br>
これらの行列を定義することによって、意図する変換を行うことができます．

\subsection moreTutoLookAt		4.1．　視点・参照点の位置の指定
視点と参照点の位置を設定できます．以下の手順に従って下さい．
\verbatim
    Affinef view;                                                            // アフィン行列
	view.Pos() = Vec3f(0.0, 15.0, 15.0);                                // 視点
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));          // 参照点、上方向ベクトル
	view = view.inv();                                                  // 逆行列を求める
	render->SetViewMatrix(view);                                        // 視点行列へ登録
\endverbatim
<br>

\subsection moreTutoMultModelMatrix		4.2．　モデル行列をかける
現在の行列に乗算する行列を指定します．
\verbatim
    render->MultModelMatrix(afw);
\endverbatim
<br>
<div align="right">
<a href="#pageMoreTutoGRSdk">top of page</a>
</div> 




*/

