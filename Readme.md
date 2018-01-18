Leptbag
======================

## 最新情報
  1/19にレポジトリをPublicに。身内用Readmeだったのでこれから随時ドキュメント等を追加する予定です。

## TREE
```
./
├── Readme.md          このファイル
├── blenderPlugin
│   └── fpmExporter.py ブレンダーで作ったモデルをjsonで出力するプラグイン
├── classes.txt        なんだこれ？？？？？？
├── friends            japariに読み込ませるプラグインを開発するフォルダ
│   ├── creativecamera マ○クラ風操作で空が飛べる
│   ├── fpscamera      一人称視点で操作する
│   ├── learningsimpledog 犬が歩行を学習する
│   ├── lowpolytree    木がたくさん生える
│   ├── planktower     ジェンガタワーが建つ
│   ├── simpledog      犬が歩く
│   ├── simplefence    適当なフェンスが出てくる
│   ├── simplefloor    床がでてくる(人気度No.1)
│   └── tpscamera      きつねを操作する
├── japariSDK          Dのライブラリを開発するところ
│   └── japarilib.d    主にjapariとのインターフェースを記述する
├── playground         各人がサンプルを使って試験する場所
└── source             プラットフォームの開発をする場所
    ├── ColorFragmentShader.fragmentshader  GLSLシェダーの一つ
    ├── TransformVertexShader.vertexshader  GLSLシェダーの一つ
    ├── bodyGenerator   物理形状(btRigidBody)を生成する
    ├── constraints     関節を生成する
    ├── elementManager  世界に配置するモデル生成器を生成する
    ├── elementNode     世界に配置されるelementそのもの
    ├── friends         プラグインをインストールする場所
    ├── makefile        これ好き
    ├── primitiveShape  cubeShapeとplaneShapeはここで定義されている
    ├── shader          GLSLのコンパイルとロードをしてくれる
    ├── utilities       べんりソースはここに
    │   ├── namedArg    メタプログラミングの賢い名前付き引数。これを使っているcppは現状ない。
    │   ├── parameterPack   パラメーターパック
    │   ├── universalString Dとc++でstringをやり取りするため
    │   ├── universalVector vec3とかquatとかの定義
    │   └── utilities   このヘッダにutilitiesのすべてのフォルダのインクルードが記述されている。
    └── vertexManager   頂点を管理してくれる

```
