ここにジャパリパークを建てよう
======================

## 最新情報
  かなり久しぶりにTreeを再生成。5/23からは変更が大きすぎて語り尽くせないが、システムとしては一新している(8/7 thotgamma)
  Dから関節も操作できるようになった。犬を学習させるサンプルも有り。次は影かなぁ... (5/23 thotgamma)  
  Dからキューブを生成できるようにした。次は関節を追加できるようにする。(5/22 thotgamma)  

## TREE
```
./
├── Readme.md          このファイル
├── blenderPlugin
│   └── fpmExporter.py ブレンダーで作ったモデルをjsonで出力するプラグイン
├── classes.txt        なんだこれ？？？？？？
├── friends            japariに読み込ませるプラグインを開発するフォルダ
│   ├── blender2bullet 
│   ├── choroDog       
│   ├── gen6DofInsect  
│   ├── learningsimpledog  てきとーな遺伝的アルゴリズムをぶち込んだ犬
│   ├── lowPolyTree    ローポリの木を表示するテスト
│   ├── plankTower     ジェンガが倒れる様を観察するテスト
│   ├── simpleFloor    床プラグイン。人気度ナンバーワン
│   ├── simpleWolf(deprecated) 
│   ├── simpledog      犬が適当に歩く
│   └── simplefence    てきとーなフェンスがスポーンする
├── japariSDK          Dのライブラリを開発するところ
│   └── japarilib.d    主にjapariとのインターフェースを記述する
├── playground         各人がサンプルを使って試験する場所
└── source             japariの開発をする場所
    ├── ColorFragmentShader.fragmentshader  GLSLシェダーの一つ
    ├── TransformVertexShader.vertexshader  GLSLシェダーの一つ
    ├── bodyGenerator   物理形状(btRigidBody)を生成する
    ├── constraints     関節を生成する
    ├── elementManager  世界に配置するモデル生成器を生成する
    ├── elementNode     世界に配置されるelementそのもの
    ├── friends         プラグインをインストールする場所
    ├── japari_park     生成物
    ├── makefile
    ├── primitiveShape  cubeShapeとplaneShapeはここで定義されている
    ├── shader          GLSLのコンパイルとロードをしてくれる
    ├── utilities       べんりソースはここに
    │   ├── namedArg    メタプログラミングの賢い名前付き引数。これを使っているcppは現状ない。
    │   ├── parameterPack   パラメーターパック
    │   ├── universalString Dとc++でstringをやり取りするため
    │   ├── universalVector vec3とかquatとかの定義
    │   └── utilities   このヘッダにutilitiesのすべてのフォルダのインクルードが記述されている。
    └── vertexManager

```
