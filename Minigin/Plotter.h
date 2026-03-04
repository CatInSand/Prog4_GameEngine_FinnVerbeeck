#ifndef PLOTTER_H
#define PLOTTER_H

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "imgui_plot.h"

#include <vector>
#include <algorithm>
#include <chrono>

namespace dae
{
	class Plotter
	{
	private:
		struct Transform {
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};
		struct IntObject {
			int ID;
		};
		struct GameObject3D {
			Transform transform;
			int ID;
		};
		struct GameObject3DAlt {
			Transform* pTransform;
			int ID;
		};

		struct PlotData
		{
			std::vector<float> m_xValues{};
			std::vector<float> m_yValues{};
			bool IsReady() { return m_xValues.size() > 0; }
		};

		int m_Plot1SampleCount{ 10 };
		PlotData m_Plot1{};

		int m_Plot2SampleCount{ 10 };
		PlotData m_Plot2{};
		PlotData m_Plot3{};

		void DrawPlot(const PlotData& plotData)
		{
			ImGui::PlotConfig conf{};
			conf.values.xs = plotData.m_xValues.data();
			conf.values.ys = plotData.m_yValues.data();
			conf.values.count = static_cast<int32_t>(plotData.m_yValues.size());
			conf.values.color = 0xFF'00'7F'FF;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element(plotData.m_yValues.begin(), plotData.m_yValues.end());
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(200, 100);
			conf.line_thickness = 2.f;

			ImGui::Plot("results", conf);
		}

		template<typename T>
		void CalculatePlot(PlotData& plotData, int sampleCount)
		{
			if (sampleCount <= 2)
			{
				assert(false);
				return;
			}

			std::vector<std::vector<float>> m_Samples{};

			//Calculate durations
			for (int index{ 0 }; index < sampleCount; ++index)
			{
				plotData.m_xValues.clear();
				plotData.m_yValues.clear();

				std::vector<std::pair<long, int>> durations{};
				std::chrono::high_resolution_clock::time_point startTime{};

				std::vector<T> arr(100000);

				for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
				{
					startTime = std::chrono::high_resolution_clock::now();

					for (size_t i{ 0 }; i < arr.size(); i += stepSize)
					{
						arr[i].ID *= 2;
					}

					durations.push_back(std::pair<long, int>{ static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()), stepSize });
				}

				for (std::pair<long, int>& duration : durations)
				{
					plotData.m_yValues.push_back(static_cast<float>(duration.first));
					plotData.m_xValues.push_back(static_cast<float>(duration.second));
				}

				m_Samples.push_back(std::move(plotData.m_yValues));
			}

			//Average out data
			plotData.m_yValues.clear();

			for (size_t iteration{ 0 }; iteration < plotData.m_xValues.size(); ++iteration)
			{
				//drop lowest and highest
				std::vector<float> samples{};
				for (int index{ 0 }; index < sampleCount; ++index)
				{
					samples.push_back(m_Samples[index][iteration]);
				}
				samples.erase(std::max_element(samples.begin(), samples.end()));
				samples.erase(std::min_element(samples.begin(), samples.end()));

				//take average
				float total{};
				for (float sample : samples)
				{
					total += sample;
				}
				plotData.m_yValues.push_back(total / (sampleCount - 2));
			}
		}

		/*void CalculatePlot1()
		{
			if (m_Plot1SampleCount <= 2)
			{
				assert(false);
				return;
			}

			std::vector<std::vector<float>> m_Samples{};

			for (int index{ 0 }; index < m_Plot1SampleCount; ++index)
			{
				m_Plot1.m_xValues.clear();
				m_Plot1.m_yValues.clear();

				std::vector<std::pair<long, int>> durations{};
				std::chrono::high_resolution_clock::time_point startTime{};

				std::vector<int> arr(100000);

				for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
				{
					startTime = std::chrono::high_resolution_clock::now();

					for (int i{ 0 }; i < arr.size(); i += stepSize)
					{
						arr[i] *= 2;
					}

					durations.push_back(std::pair<long, int>{ static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()), stepSize });
				}

				for (std::pair<long, int>& duration : durations)
				{
					m_Plot1.m_yValues.push_back(static_cast<float>(duration.first));
					m_Plot1.m_xValues.push_back(static_cast<float>(duration.second));
				}

				m_Samples.push_back(std::move(m_Plot1.m_yValues));
			}

			m_Plot1.m_yValues.clear();

			for (int iteration{ 0 }; iteration < m_Plot1.m_xValues.size(); ++iteration)
			{
				//drop lowest and highest
				std::vector<float> samples{};
				for (int index{ 0 }; index < m_Plot1SampleCount; ++index)
				{
					samples.push_back(m_Samples[index][iteration]);
				}
				samples.erase(std::max_element(samples.begin(), samples.end()));
				samples.erase(std::min_element(samples.begin(), samples.end()));

				//take average
				float total{};
				for (float sample : samples)
				{
					total += sample;
				}
				m_Plot1.m_yValues.push_back(total / (m_Plot1SampleCount - 2));
			}
		}
		void CalculatePlot2()
		{
			if (m_Plot2SampleCount <= 2)
			{
				assert(false);
				return;
			}

			std::vector<std::vector<float>> m_Samples{};

			for (int index{ 0 }; index < m_Plot2SampleCount; ++index)
			{
				m_Plot2.m_xValues.clear();
				m_Plot2.m_yValues.clear();

				std::vector<std::pair<long, int>> durations{};
				std::chrono::high_resolution_clock::time_point startTime{};

				std::vector<GameObject3D> arr(100000);

				for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
				{
					startTime = std::chrono::high_resolution_clock::now();

					for (int i{ 0 }; i < arr.size(); i += stepSize)
					{
						arr[i].ID *= 2;
					}

					durations.push_back(std::pair<long, int>{ static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()), stepSize });
				}

				for (std::pair<long, int>& duration : durations)
				{
					m_Plot2.m_yValues.push_back(static_cast<float>(duration.first));
					m_Plot2.m_xValues.push_back(static_cast<float>(duration.second));
				}

				m_Samples.push_back(std::move(m_Plot2.m_yValues));
			}

			m_Plot2.m_yValues.clear();

			for (int iteration{ 0 }; iteration < m_Plot2.m_xValues.size(); ++iteration)
			{
				//drop lowest and highest
				std::vector<float> samples{};
				for (int index{ 0 }; index < m_Plot2SampleCount; ++index)
				{
					samples.push_back(m_Samples[index][iteration]);
				}
				samples.erase(std::max_element(samples.begin(), samples.end()));
				samples.erase(std::min_element(samples.begin(), samples.end()));

				//take average
				float total{};
				for (float sample : samples)
				{
					total += sample;
				}
				m_Plot2.m_yValues.push_back(total / (m_Plot2SampleCount - 2));
			}
		}
		void CalculatePlot3()
		{
			if (m_Plot2SampleCount <= 2)
			{
				assert(false);
				return;
			}

			std::vector<std::vector<float>> m_Samples{};

			for (int index{ 0 }; index < m_Plot2SampleCount; ++index)
			{
				m_Plot3.m_xValues.clear();
				m_Plot3.m_yValues.clear();

				std::vector<std::pair<long, int>> durations{};
				std::chrono::high_resolution_clock::time_point startTime{};

				std::vector<GameObject3D> arr(100000);

				for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
				{
					startTime = std::chrono::high_resolution_clock::now();

					for (int i{ 0 }; i < arr.size(); i += stepSize)
					{
						arr[i].ID *= 2;
					}

					durations.push_back(std::pair<long, int>{ static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()), stepSize });
				}

				for (std::pair<long, int>& duration : durations)
				{
					m_Plot3.m_yValues.push_back(static_cast<float>(duration.first));
					m_Plot3.m_xValues.push_back(static_cast<float>(duration.second));
				}

				m_Samples.push_back(std::move(m_Plot3.m_yValues));
			}

			m_Plot3.m_yValues.clear();

			for (int iteration{ 0 }; iteration < m_Plot3.m_xValues.size(); ++iteration)
			{
				//drop lowest and highest
				std::vector<float> samples{};
				for (int index{ 0 }; index < m_Plot2SampleCount; ++index)
				{
					samples.push_back(m_Samples[index][iteration]);
				}
				samples.erase(std::max_element(samples.begin(), samples.end()));
				samples.erase(std::min_element(samples.begin(), samples.end()));

				//take average
				float total{};
				for (float sample : samples)
				{
					total += sample;
				}
				m_Plot3.m_yValues.push_back(total / (m_Plot2SampleCount - 2));
			}
		}*/

	public:
		Plotter() = default;

		void Draw()
		{
			ImGui::Begin("Exercise 1");
			ImGui::InputInt("# samples", &m_Plot1SampleCount);
			if (ImGui::Button("Calculate plot 1"))
				CalculatePlot<dae::Plotter::IntObject>(m_Plot1, m_Plot1SampleCount);
			if(m_Plot1.IsReady())
				DrawPlot(m_Plot1);
			ImGui::End();


			ImGui::Begin("Exercise 2");
			ImGui::InputInt("# samples", &m_Plot2SampleCount);
			if (ImGui::Button("Calculate GameObject3D plot"))
				CalculatePlot<dae::Plotter::GameObject3D>(m_Plot2, m_Plot2SampleCount);
			if(m_Plot2.IsReady())
				DrawPlot(m_Plot2);
			if (ImGui::Button("Calculate GameObject3DAlt plot"))
				CalculatePlot<dae::Plotter::GameObject3DAlt>(m_Plot3, m_Plot2SampleCount);
			if (m_Plot3.IsReady())
				DrawPlot(m_Plot3);
			ImGui::End();
		}
	};
}

#endif