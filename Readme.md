ここにジャパリパークを建てよう
======================

## 最新情報
  Dから関節も操作できるようになった。犬を学習させるサンプルも有り。次は影かなぁ... (5/23 thotgamma)  
  Dからキューブを生成できるようにした。次は関節を追加できるようにする。(5/22 thotgamma)  

## TREE
```
japari/
├── Readme.md  このファイル
├── playground 試験用のコードを書くところ
│   ├── sanamichoro
│   └── thotgamma
│       ├── IM_SO_SMART    C++とDをリンクするテスト
│       ├── bulletTest     bulletを簡単に試してみるテスト
│       ├── glfwTest       glfwで簡単にキューブを描画するテスト
│       ├── greatOpenGL    なんかすごい
│       ├── instancing     opgnelでいっぱい描く
│       ├── manymanycubes  キューブがとにかくいっぱい出てくる
│       └── visualbullet   bulletのオブジェクトを描画するテスト
│
└── source  実際にjapariとして動かす本番用ファイル
    ├── makefile  プロジェクト全体をビルドするためのファイル
    ├── ColorFragmentShader.fragmentshader  グラボで描画するために必要なファイル
    ├── TransformVertexShader.vertexshader  グラボで描画するために必要なファイル
    ├── constraints.cpp  関節 [Bullet]
    ├── constraints.hpp
    ├── cubeshape.cpp    キューブ [Bullet]
    ├── cubeshape.hpp
    ├── floorshape.cpp   床 [Bullet]
    ├── floorshape.hpp
    ├── main.cpp         MAIN
    ├── shader.cpp       シェーダを読み込むため[Opengl]
    ├── shader.hpp
    ├── vertexmanage.cpp 頂点を管理する[Opengl]
    ├── vertexmanage.hpp
    ├── visualBullet     EXECUTABLE
    └── plugins  この中に動物など、アプリケーション上で動かす物体を作る
        ├── dog.d
        ├── libdog.so
        └── makedog.sh  dogをビルドするためのスクリプト
```
