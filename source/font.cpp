#include "font.hpp"


std::unordered_set<textbox*> textbox::instances;

// # std::unordered_map<int, textbox*> textbox::ownerList
// どのテキストボックスが描画リスト何番目の文字を所有しているのかを管理する
// 描画リストの並び替え(削除時に発生する)の際、持ち主テキストボックスに通知するのに使用する
std::unordered_map<int, textbox*> textbox::ownerList;


textbox::textbox(std::u16string text, int x, int y, int size, int r, int g, int b):
											text(text), x(x), y(y), size(size), r(r), g(g), b(b) {
	this->instances.insert(this);
	length = text.length();
	render();
}

textbox::~textbox() {
	this->instances.erase(this);
}

/*
extern "C" textbox_interface* createTextbox_interface(char16_t* text, int length, int x, int y, int size, int r, int g, int b) {
	return new textbox(std::u16string(text, length), x, y, size, r, g, b);
}
*/

// 要求のあった文字列を再現する為、一文字一文字をきれいに並べて描画リストに挿入する。
void textbox::render() {
	float originX         = ((float)x/(float)font::windowWidth)*2.0f - 1.0f;
	float originY         = ((float)y/(float)font::windowWidth)*2.0f - 1.0f;
	float fontScaleWidth  = (float)size/(float)font::windowWidth * 2;
	float fontScaleHeight = (float)size/(float)font::windowHeight * 2;

	float normalR = (float)r/255.0f;
	float normalG = (float)g/255.0f;
	float normalB = (float)b/255.0f;

	// このクラス(textbox)が管理する文字が文字描画リストの何番目に今あるのかを管理する。
	characterIDArray = std::unique_ptr<int[]>(new int[text.length()]);

	for (int i = 0; i < length; i++) {

		// 文字の大きさ等を計算
		auto info = font::getCharInfo(text[i]);

		float scaledWidth        = fontScaleWidth  * ((float)info.width        / (float)font::textureHeight);
		float scaledHeight       = fontScaleHeight * ((float)info.height       / (float)font::textureHeight);
		float scaledBearingX     = fontScaleWidth  * ((float)info.bearingX     / (float)font::textureHeight);
		float scaledBearingY     = fontScaleHeight * ((float)info.bearingY     / (float)font::textureHeight);
		float scaledAdvanceWidth = fontScaleWidth  * (((float)info.advanceWidth/64.0f) / (float)font::textureHeight);

		float aX = originX + scaledBearingX;
		float aY = originY + scaledBearingY - scaledHeight;
		float eX = aX + scaledWidth;
		float eY = aY + scaledHeight;

		// 描画リストに追加
		int charID = 	font::addCharacterToDrawList({
									{aX, eY, info.u01, info.v01, normalR, normalG, normalB},
									{aX, aY, info.u00, info.v00, normalR, normalG, normalB},
									{eX, aY, info.u10, info.v10, normalR, normalG, normalB},

									{aX, eY, info.u01, info.v01, normalR, normalG, normalB},
									{eX, aY, info.u10, info.v10, normalR, normalG, normalB},
									{eX, eY, info.u11, info.v11, normalR, normalG, normalB}
								});

		// 所有者リストに追加
		characterIDArray[i] = charID;
		ownerList[charID] = this;

		originX += scaledAdvanceWidth;
	}

	font::reloadVBO();
}

void textbox::rerender() {
	destroy();
	render();
}

// --以下テクストボックスの更新系。めんどくさいのでプロパティを適切に変更した後、--------┐
// 文字を全部削除して追加し直している。(本当は一部のプロパティを書き換えるだけでOK) #TODO
void textbox::updateText(char16_t *text, int length) {
	destroy();
	this->text = std::u16string(text, length);
	this->length = length;
	render();
}

void textbox::updateText(std::u16string newtext) {
	destroy();
	this->text = newtext;
	this->length = newtext.size();
	render();
}

void textbox::updateColor(int newR, int newG, int newB) {
	destroy();
	r = newR;
	g = newG;
	b = newB;
	render();
}

void textbox::updateSize(int newSize) {
	destroy();
	size = newSize;
	render();
}

void textbox::updatePos(int newX, int newY) {
	destroy();
	x = newX;
	y = newY;
	render();
}
//---------------------------------------------------------------------------------------┘


// 描画リスト内で並び替えが発生した際呼ばれる
void textbox::updateID(int before, int after) {
	for (int i = 0; i < length; i++) {
		if (characterIDArray[i] == before) {
			characterIDArray[i] = after;
			return;
		}
	}

	// 所有者リストにそのIDの持ち主は自分だと記載してあるのに、
	// こちらではそれを確認できない際にここに来る(プログラミングが下手)
	std::cout << "program Error(ecaohfmoefhaouwhf)" << std::endl;
}

void textbox::destroy() {
	// 例によって末尾の要素を削除する要素に上書きし、pop_backしている。
	for (int i = 0; i < length; i++) {
		int target = characterIDArray[i];
		int alterdID = font::removeCharacterFromDrawList(target);
		ownerList[target] = ownerList[alterdID];
		ownerList[alterdID]->updateID(alterdID, target);
	}
	font::reloadVBO();
}

namespace font {

	int windowWidth;
	int windowHeight;

	GLuint textAtlas;

	FT_Library ft;
	FT_Face face;

	GLuint program_TEXT;
	GLuint characterVAO, characterVBO;

	GLuint textAtlasItr = 0;
	GLuint textAtlasWidth = 512;
	std::vector<character> characterVector;
	std::unordered_map<char16_t, charInfo> charMap;

	//字形情報を返す。フォントレンダからの字形登録までやってくれる。
	charInfo getCharInfo(char16_t request) {

		// 要求された文字が登録済みかどうか調べる
		if (auto iter = charMap.find(request); iter != end(charMap)) {
			// 登録済みであればそれを返す
			return iter->second;
		} else {
			// 新たに登録する
			// FreeTypeで描画する。
			FT_Load_Glyph(face, FT_Get_Char_Index(face, request), FT_LOAD_RENDER);
			const auto &glyph = face->glyph;

			// もし今のテクスチャアトラスに空きがなければ、テクスチャアトラスを拡張する。
			while (textAtlasItr + glyph->bitmap.width > textAtlasWidth) {
				expandTexture();
			}

			// FreeTypeの描画結果をアトラステクスチャに書き込む。
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, textAtlas);
			glTexSubImage2D(GL_TEXTURE_2D, 0, textAtlasItr, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

			// FreeTypeの座標系とOpenGLの座標系は違うので順番に注意してUV座標を格納する。
			// そのままやると文字が逆さまになる。
			charInfo newchar = {(float)textAtlasItr/(float)textAtlasWidth,							(float)glyph->bitmap.rows/(float)textureHeight, //01
								(float)textAtlasItr/(float)textAtlasWidth,							0,												//00
								(float)(textAtlasItr + glyph->bitmap.width)/(float)textAtlasWidth,	(float)glyph->bitmap.rows/(float)textureHeight, //11
								(float)(textAtlasItr + glyph->bitmap.width)/(float)textAtlasWidth,	0,												//10
								(GLuint)glyph->bitmap.width,
								(GLuint)glyph->bitmap.rows,
								(GLuint)glyph->bitmap_left,
								(GLuint)glyph->bitmap_top,
								(GLuint)glyph->advance.x};

			// 今書き込んだ文字の幅だけ次の書き込みポインタをずらす
			textAtlasItr += glyph->bitmap.width;

			// 今描画したものを描画済みとしてcharMapに登録しておく。
			charMap[request] = newchar;
			return newchar;
		}
	}


	// 描画する文字リストを実際にGPUに送る。
	void reloadVBO() {
		glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
		glBufferData(GL_ARRAY_BUFFER, characterVector.size() * sizeof(character), &characterVector[0], GL_STATIC_DRAW);
	}


	// アトラステクスチャを2倍の幅に拡張する。
	void expandTexture() {

		// 拡張するにあたり、一度今の情報を読み出して、拡張してから戻す。
		glBindTexture(GL_TEXTURE_2D, textAtlas);
		GLubyte *buff = new GLubyte[textAtlasWidth*textureHeight];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, buff);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textAtlasWidth*2, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textAtlasWidth, textureHeight, GL_RED, GL_UNSIGNED_BYTE, buff);
		delete buff;

		// 描画リストに登録されてる各文字のUV座標のU(横方向)を1/2にする。(テクスチャの幅が二倍になったので)
		for (auto& character : characterVector) {
			character.v0.u /= 2.0f;
			character.v1.u /= 2.0f;
			character.v2.u /= 2.0f;
			character.v3.u /= 2.0f;
			character.v4.u /= 2.0f;
			character.v5.u /= 2.0f;
		}

		// 文字情報リストに登録されている各文字のUV座標のU(ry
		for (auto itr = charMap.begin(); itr != charMap.end(); itr++) {
			auto& elem = itr->second;
			elem.u00 /= 2;
			elem.u01 /= 2;
			elem.u10 /= 2;
			elem.u11 /= 2;
		}

		textAtlasWidth *= 2;

		reloadVBO();
	}

	// characterを投げるとリストに追加してくれる。
	// その時、今登録したcharacterが、リストの何番目に入ったのか通知してくれる。
	int addCharacterToDrawList(character request) {
		int id = characterVector.size();
		characterVector.push_back(request);
		return id;
	}

	// id番目のcharacterをリストから消去する。
	// その時、削除した場所に移動されたcharacterの元idを返す。
	// この関数を呼んだときは、ownerListを使って移動されたcharacterの
	// 持ち主textBoxに通知しなければならない。
	int removeCharacterFromDrawList(int id) {
		characterVector[id] = characterVector.back();
		characterVector.pop_back();
		return characterVector.size();
	}

	// 呼んで。まず。
	void setup(int windowWidth, int windowHeight) {

		font::windowWidth = windowWidth;
		font::windowHeight = windowHeight;


		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

		// あ、使いたいフォントパスをここに書いて(setupでなげれるようにするのもアリ)
		if (FT_New_Face(ft, "/System/Library/Fonts/ヒラギノ角ゴシック W4.ttc", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  

		FT_Set_Pixel_Sizes(face, 0, textureHeight);

		glGenTextures(1, &textAtlas);
		glBindTexture(GL_TEXTURE_2D, textAtlas);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textAtlasWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// テキスト描画用のシェーダ
		program_TEXT = LoadShaders("text.vert", "text.frag");

		// 描画リストの構造をここでGPUに伝えている。
		// 構造は[x, y, u, v, r, g, b] (というかstruct xyuvrgb)
		glGenVertexArrays(1, &characterVAO);
		glGenBuffers(1, &characterVBO);
		glBindVertexArray(characterVAO);
		glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
		glBufferData(GL_ARRAY_BUFFER, characterVector.size() * sizeof(character), &characterVector[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(xyuvrgb), (void*)0);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(xyuvrgb), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(xyuvrgb), (void*)(4 * sizeof(float)));

		// フォントは背景透過したいからねー
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void updateWindowSize(int newWidth, int newHeight) {
		font::windowWidth = newWidth;
		font::windowHeight = newHeight;

		for (auto elem : textbox::instances) {
			elem->rerender();
		}
	}

	// 実際に画面に描画する。
	void draw() {
		glUseProgram(program_TEXT);

		// フォントは背景透過したいね (プログラム通してEnableするなら消して)
		glEnable(GL_BLEND);
		glBindVertexArray(characterVAO);
		glBindTexture(GL_TEXTURE_2D, textAtlas);
		glDrawArrays(GL_TRIANGLES, 0, characterVector.size() * 6);
		glDisable(GL_BLEND);

	}
}


