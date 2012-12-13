#pragma once

#include "About.h"
#include "Settings.h"
#include "Preview.h"
#include "Playback.h"
#include "JpegReader.h"
#include "bwWebServer.h"

namespace Birdwatcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(CFilterGraph *pGraph, CWebServer *pWebServer)
		{
			m_pGraph = pGraph;
			m_pWebServer = pWebServer;
			InitializeComponent();
			m_previewForm = gcnew Preview();

		}
		void Init(void)
		{
			m_pGraph->BuildGraph();
			
			if(m_pGraph->m_NbVideoSources == 0)
			{
				m_pGraph->SetEnablePreview(false);
				//this->ClientSize = System::Drawing::Size(350, 80);
				this->noVideoLabel->Visible = true;
				this->previewCheckBox->Enabled = false;
				this->startstop->Enabled = false;
			}
			else
			{
				bool previewEnabled = m_pGraph->GetEnablePreview();
				if(previewEnabled)
				{
					LinkRendererToPreviewForm();
					this->previewCheckBox->Checked = true;
					m_pGraph->EnableRenderer(true);
					m_previewForm->Show();
				}
				else
				{
					this->previewCheckBox->Checked = false;
					m_previewForm->Hide();
					m_pGraph->EnableRenderer(false);
				}
			}
			m_bRunning = FALSE;


			//m_pGraph->SetEnableFtpUpload(FALSE);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		 CFilterGraph *m_pGraph;
		 CWebServer *m_pWebServer;

	private: System::Windows::Forms::Button^  startstop;
	protected: 

	protected: 

	private:
		Preview^ m_previewForm;
		/// <summary>



	private: System::Windows::Forms::Button^  about_button;
	private: System::Windows::Forms::Button^  settings_button;


	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  noVideoLabel;
	private: System::Windows::Forms::CheckBox^  previewCheckBox;

	private: System::Windows::Forms::Button^  playbackButton;

			 BOOL m_bRunning;
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.

		void LinkRendererToPreviewForm()
		{
			LONG w = 0, h = 0;
			m_pGraph->GetVideoSize(&w, &h);
			m_previewForm->ClientSize = System::Drawing::Size(w, h);
			m_pGraph->SetOwner(reinterpret_cast<HWND>(m_previewForm->Handle.ToPointer()), 0, 0, w, h);
		}
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->startstop = (gcnew System::Windows::Forms::Button());
			this->about_button = (gcnew System::Windows::Forms::Button());
			this->settings_button = (gcnew System::Windows::Forms::Button());
			this->noVideoLabel = (gcnew System::Windows::Forms::Label());
			this->previewCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->playbackButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// startstop
			// 
			this->startstop->Location = System::Drawing::Point(91, 12);
			this->startstop->Name = L"startstop";
			this->startstop->Size = System::Drawing::Size(75, 23);
			this->startstop->TabIndex = 0;
			this->startstop->Text = L"RUN";
			this->startstop->UseVisualStyleBackColor = true;
			this->startstop->Click += gcnew System::EventHandler(this, &Form1::startstop_Click);
			// 
			// about_button
			// 
			this->about_button->Location = System::Drawing::Point(334, 12);
			this->about_button->Name = L"about_button";
			this->about_button->Size = System::Drawing::Size(75, 23);
			this->about_button->TabIndex = 2;
			this->about_button->Text = L"About";
			this->about_button->UseVisualStyleBackColor = true;
			this->about_button->Click += gcnew System::EventHandler(this, &Form1::about_button_Click);
			// 
			// settings_button
			// 
			this->settings_button->Location = System::Drawing::Point(172, 12);
			this->settings_button->Name = L"settings_button";
			this->settings_button->Size = System::Drawing::Size(75, 23);
			this->settings_button->TabIndex = 3;
			this->settings_button->Text = L"Settings";
			this->settings_button->UseVisualStyleBackColor = true;
			this->settings_button->Click += gcnew System::EventHandler(this, &Form1::settings_button_Click);
			// 
			// noVideoLabel
			// 
			this->noVideoLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->noVideoLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->noVideoLabel->ForeColor = System::Drawing::Color::Red;
			this->noVideoLabel->Location = System::Drawing::Point(12, 42);
			this->noVideoLabel->Name = L"noVideoLabel";
			this->noVideoLabel->Size = System::Drawing::Size(397, 26);
			this->noVideoLabel->TabIndex = 4;
			this->noVideoLabel->Text = L"No video capture device found";
			this->noVideoLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->noVideoLabel->Visible = false;
			// 
			// previewCheckBox
			// 
			this->previewCheckBox->AutoSize = true;
			this->previewCheckBox->Location = System::Drawing::Point(12, 16);
			this->previewCheckBox->Name = L"previewCheckBox";
			this->previewCheckBox->Size = System::Drawing::Size(64, 17);
			this->previewCheckBox->TabIndex = 5;
			this->previewCheckBox->Text = L"Preview";
			this->previewCheckBox->UseVisualStyleBackColor = true;
			this->previewCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::previewCheckBox_CheckedChanged);
			// 
			// playbackButton
			// 
			this->playbackButton->Location = System::Drawing::Point(253, 12);
			this->playbackButton->Name = L"playbackButton";
			this->playbackButton->Size = System::Drawing::Size(75, 23);
			this->playbackButton->TabIndex = 6;
			this->playbackButton->Text = L"Playback";
			this->playbackButton->UseVisualStyleBackColor = true;
			this->playbackButton->Click += gcnew System::EventHandler(this, &Form1::playbackButton_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(421, 85);
			this->Controls->Add(this->playbackButton);
			this->Controls->Add(this->previewCheckBox);
			this->Controls->Add(this->noVideoLabel);
			this->Controls->Add(this->about_button);
			this->Controls->Add(this->settings_button);
			this->Controls->Add(this->startstop);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->Text = L"Birdwatcher";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void startstop_Click(System::Object^  sender, System::EventArgs^  e) {
				if(!m_bRunning) // stopped
				{
					m_pGraph->Run();
					m_bRunning = TRUE;
					this->startstop->Text = L"STOP";
				}
				else
				{
					m_pGraph->Stop();
					m_bRunning = FALSE;
					this->startstop->Text = L"RUN";
				}
			 }
	private: System::Void about_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 About^ newMDIChild = gcnew About();
				 newMDIChild->ShowDialog();
			 }
	private: System::Void settings_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 Settings^ newMDIChild = gcnew Settings(m_pGraph, m_pWebServer);
				 if(newMDIChild->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 LinkRendererToPreviewForm();
					 if(m_bRunning)
						 m_pGraph->Run();
				 }
			}
private: System::Void previewCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if(this->previewCheckBox->Checked)
			{
				m_previewForm->StartPosition = FormStartPosition::Manual;
				m_previewForm->Location = System::Drawing::Point(this->Location.X, this->Location.Y+this->Size.Height);

				m_pGraph->Stop();
				m_pGraph->SetEnablePreview(true);
				m_pGraph->Reconnect();
				LinkRendererToPreviewForm();
				m_pGraph->EnableRenderer(true);
				if(m_bRunning)
					m_pGraph->Run();

				m_previewForm->Show();
			}
			else
			{
				m_previewForm->Hide();
				m_pGraph->EnableRenderer(false);
				m_pGraph->Stop();
				m_pGraph->SetEnablePreview(false);
				m_pGraph->Reconnect();
			}
		 }
private: System::Void playbackButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 Playback^ pPlaybackForm = gcnew Playback(m_pGraph);
			 pPlaybackForm->Show();
		 }
};
}

