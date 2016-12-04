//----------------------------------------------------------------------------------------
//
//	Siv3D Asset Loader (experimental)
//
//	Copyright (C) 2016 Ryo Suzuki
//
//	Licensed under the MIT License.
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------

# pragma once
# include <ppl.h>
# include <ppltasks.h>
# include <Siv3D.hpp>

namespace s3d
{
	namespace experimental
	{
		template <class AssetType, class AssetDataType>
		class AssetLoader
		{
		private:

			Array<FilePath> m_paths;

			Array<AssetDataType> m_assetData;

			Array<concurrency::task<void>> m_tasks;

			Array<AssetType> m_assets;

			Array<bool> m_states;

			uint32 m_count_done = 0;

			bool m_isActive = false;

		public:

			AssetLoader() = default;

			AssetLoader(const Array<FilePath>& paths, bool startImmediately)
				: m_paths(paths)
				, m_assetData(paths.size())
				, m_assets(paths.size())
				, m_states(paths.size())
			{
				if (startImmediately)
				{
					start();
				}
			}

			~AssetLoader()
			{
				wait_all();
			}

			void start()
			{
				if (m_isActive)
				{
					return;
				}

				for (size_t i = 0; i < m_paths.size(); ++i)
				{
					m_tasks.emplace_back(concurrency::create_task(
						[&path = m_paths[i], &assetData = m_assetData[i]]()
					{
						assetData = AssetDataType(path);
					}));
				}

				m_isActive = true;

				return;
			}

			void update(const int32 maxCreationPerFrame = 4)
			{
				if (!m_isActive || done())
				{
					return;
				}

				const int32 max = std::max(1, maxCreationPerFrame);
				int32 n = 0;

				for (size_t i = 0; i < m_paths.size(); ++i)
				{
					if (!m_states[i] && m_tasks[i].is_done())
					{
						m_assets[i] = AssetType(m_assetData[i]);

						m_assetData[i].release();

						m_states[i] = true;

						++m_count_done;

						if (++n >= maxCreationPerFrame)
						{
							break;
						}
					}
				}
			}

			size_t num_loaded() const
			{
				return m_count_done;
			}

			size_t size() const
			{
				return m_paths.size();
			}

			void wait_all()
			{
				when_all(std::begin(m_tasks), std::end(m_tasks)).wait();
			}

			bool isActive()
			{
				return m_isActive;
			}

			bool done() const
			{
				return num_loaded() == size();
			}

			const Array<bool>& getStates() const
			{
				return m_states;
			}

			bool getState(const size_t index) const
			{
				return m_states[index];
			}

			const Array<AssetType>& getAssets() const
			{
				return m_assets;
			}

			const AssetType& getAsset(const size_t index) const
			{
				return m_assets[index];
			}
		};

		using TextureLoader = AssetLoader<Texture, Image>;
		using SoundLoader = AssetLoader<Sound, Wave>;
	}
}
