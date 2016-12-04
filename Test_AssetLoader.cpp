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

namespace test
{
	bool HasImageFileExtension(const FilePath& path)
	{
		static const Array<String> imageExtensions =
		{
			L"png", L"jpg", L"jpeg", L"bmp", L"jp2", L"dds", L"webp", L"gif", L"tif", L"tiff", L"tga", L"ppm"
		};

		const String extension = FileSystem::Extension(path);

		return std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end();
	}

	bool HasAudioFileExtension(const FilePath& path)
	{
		static const Array<String> audioExtensions =
		{
			L"wav", L"mp3", L"ogg", L"opus", L"m4a", L"wma"
		};

		const String extension = FileSystem::Extension(path);

		return std::find(audioExtensions.begin(), audioExtensions.end(), extension) != audioExtensions.end();
	}

	void TestTextureLoader()
	{
		Array<FilePath> paths;

		// どのフォルダの画像ファイルを開くかを選択
		if (const auto directory = Dialog::GetFolder())
		{
			for (const auto& path : FileSystem::DirectoryContents(directory.value()))
			{
				if (FileSystem::IsFile(path) && HasImageFileExtension(path))
				{
					paths.push_back(path);
				}
			}
		}

		Stopwatch stopwatch(true);

		const bool startImmediately = true;

		// バックグラウンド & マルチスレッドで、paths で指定した画像を読み込み開始
		experimental::TextureLoader loader = experimental::TextureLoader(paths, startImmediately);

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
				Println(L"ロード完了: {}/{} / {}ms"_fmt, loader.num_loaded(), loader.size(), stopwatch.ms());

				stopwatch.reset();
			}

			//// ロードしたテクスチャを描画。ロード中は代わりに四角を表示
			//for (int32 i : step(static_cast<int32>(loader.size())))
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

		// どのフォルダの音楽ファイルを開くかを選択
		if (const auto directory = Dialog::GetFolder())
		{
			for (const auto& path : FileSystem::DirectoryContents(directory.value()))
			{
				if (FileSystem::IsFile(path) && HasAudioFileExtension(path))
				{
					paths.push_back(path);
				}
			}
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
				Println(L"ロード完了: {}/{} / {}ms"_fmt, loader.num_loaded(), loader.size(), stopwatch.ms());

				stopwatch.reset();
			}

			//// ロード完了した Sound の再生ボタンを表示。ロード中は枠のみ。
			//for (int32 i : step(static_cast<int32>(loader.size())))
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
}

void Main()
{
	test::TestTextureLoader();

	//test::TestSoundLoader();
}
