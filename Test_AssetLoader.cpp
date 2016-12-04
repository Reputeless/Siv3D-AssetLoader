//--------------------------------------------------------------------
//
//	Siv3D Asset Loader (experimental)
//
//	Copyright (C) 2016 Ryo Suzuki <reputeless@gmail.com>
//
//	For the license information refer to AssetLoader.hpp.
//
//--------------------------------------------------------------------

# include <Siv3D.hpp>
# include "AssetLoader.hpp"

void TestTextureLoader()
{
	Array<FilePath> paths;

	for (const auto& path : FileSystem::DirectoryContents(Dialog::GetFolder().value()))
	{
		paths.push_back(path);
	}

	Stopwatch stopwatch(true);

	const bool startImmediately = true;

	// バックグラウンド & マルチスレッドで、paths で指定した画像を読み込み開始
	experimental::TextureLoader loader(paths, startImmediately);

	while (System::Update())
	{
		if (loader.isActive() && !loader.done())
		{
			// ロードが完了した画像から順次 Texture を作成する。
			// 1 回の upadate で作成する Texture の最大数を少なくすると、フレームレートの低下を防げるが、
			// 最大数が 1 だと、100 枚の Texture を作成するには最低でも 100 フレーム必要になる。
			const int32 maxCreationPerFrame = 1;
			loader.update(maxCreationPerFrame);
		}

		if (!loader.done())
		{
			Println(L"ロード中: {}/{}"_fmt, loader.num_loaded(), loader.size());
		}
		else if (stopwatch.isActive())
		{
			Println(L"ロード完了: {}ms"_fmt, stopwatch.ms());

			stopwatch.reset();
		}

		//// ロードしたテクスチャを描画。ロード中は代わりに四角を表示
		//for (size_t i : step(loader.size()))
		//{
		//	const Rect area(20 + (i % 6) * 100, 20 + (i / 6) * 80, 80, 60);

		//	if (loader.getState(i))
		//	{
		//		area(loader.getAsset(i)).draw();
		//	}
		//	else
		//	{
		//		area.draw(Color(50));
		//	}
		//}
	}
}

void TestSoundLoader()
{
	Array<FilePath> paths;

	for (const auto& path : FileSystem::DirectoryContents(Dialog::GetFolder().value()))
	{
		paths.push_back(path);
	}

	Stopwatch stopwatch(true);

	const bool startImmediately = true;

	// バックグラウンド & マルチスレッドで、paths で指定した音楽を読み込み開始
	experimental::SoundLoader loader(paths, startImmediately);

	while (System::Update())
	{
		if (loader.isActive() && !loader.done())
		{
			// ロードが完了した音楽から順次 Sound を作成する。
			// 1 回の upadate で作成する Sound の最大数を少なくすると、フレームレートの低下を防げるが、
			// 最大数が 1 だと、100 個の Sound を作成するには最低でも 100 フレーム必要になる。
			const int32 maxCreationPerFrame = 1;
			loader.update(maxCreationPerFrame);
		}

		if (!loader.done())
		{
			Println(L"ロード中: {}/{}"_fmt, loader.num_loaded(), loader.size());
		}
		else if (stopwatch.isActive())
		{
			Println(L"ロード完了: {}ms"_fmt, stopwatch.ms());

			stopwatch.reset();
		}

		//// ロード完了した Sound の再生ボタンを表示。ロード中は枠のみ。
		//for (size_t i : step(loader.size()))
		//{
		//	const Rect area(20 + (i % 6) * 100, 20 + (i / 6) * 80, 80, 60);

		//	if (loader.getState(i))
		//	{
		//		area.draw();

		//		Triangle(area.center, 30, 90_deg).moveBy(-2, 0).draw(Palette::Deepskyblue);

		//		if (area.leftClicked)
		//		{
		//			loader.getAsset(i).play();
		//		}
		//	}
		//	else
		//	{
		//		area.drawFrame(2, 0, Color(120));
		//	}
		//}
	}
}

void Main()
{
	TestTextureLoader();
}
