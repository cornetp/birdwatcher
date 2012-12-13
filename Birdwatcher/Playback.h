#pragma once

#include "FilterGraph.h"
#include "JpegReader.h"
#include "Debug.h"

namespace Birdwatcher {

	const LONG		ONE_MSEC = 10000;   // The number of 100-ns in 1 msec
	const UINT		TICK_FREQ = 200;	// Timer frequency in msec

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Globalization;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Playback
	/// </summary>
	public ref class Playback : public System::Windows::Forms::Form
	{
	public:
		Playback(CFilterGraph *pGraph)
		{
			m_pGraph = pGraph;
			InitializeComponent();
			InitForm();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Playback()
		{
			if(m_pJpegReader) 
				delete(m_pJpegReader);
			if (components)
			{
				delete components;
			}
		}
	private: CFilterGraph *m_pGraph;
	private: System::Windows::Forms::ToolStripButton^  pauseButton;
	private: System::Windows::Forms::TrackBar^  timeTrackBar;
	private: System::Windows::Forms::MonthCalendar^  monthCalendar;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker;
	private: System::Windows::Forms::CheckBox^  showCalendarCheckBox;
	private: System::Windows::Forms::Timer^  trackBarTimer;
	private: System::Windows::Forms::Label^  timeStampLabel;
	private: System::Windows::Forms::Button^  stillImagesFolderButton;
	private: System::Windows::Forms::TextBox^  stillImagesFolderTextBox;
	private: System::Windows::Forms::Label^  stillImagesDirectoryLabel;
	private: System::Windows::Forms::FolderBrowserDialog^  stillFolderBrowserDialog;
	private: System::Windows::Forms::ToolStripButton^  Record;

	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;




	protected: 
	private: CJpegReader * m_pJpegReader;
	private: void InitForm(void);
	private: void BrowseStillFolder(void);
	private: void SetStillFolder(void);
	private: void StopPlayback(void);

	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripButton^  playButton;
	private: System::Windows::Forms::Panel^  videoPanel;


	private: System::Windows::Forms::ToolStripButton^  stopButton;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Playback::typeid));
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->stopButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->pauseButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->playButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->Record = (gcnew System::Windows::Forms::ToolStripButton());
			this->videoPanel = (gcnew System::Windows::Forms::Panel());
			this->timeTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->monthCalendar = (gcnew System::Windows::Forms::MonthCalendar());
			this->dateTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
			this->showCalendarCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->trackBarTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->timeStampLabel = (gcnew System::Windows::Forms::Label());
			this->stillImagesFolderButton = (gcnew System::Windows::Forms::Button());
			this->stillImagesFolderTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stillImagesDirectoryLabel = (gcnew System::Windows::Forms::Label());
			this->stillFolderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->toolStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->timeTrackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->stopButton, this->pauseButton, 
				this->playButton, this->Record});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(310, 25);
			this->toolStrip1->TabIndex = 0;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// stopButton
			// 
			this->stopButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->stopButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"stopButton.Image")));
			this->stopButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(23, 22);
			this->stopButton->Text = L"Stop";
			this->stopButton->Click += gcnew System::EventHandler(this, &Playback::stopButton_Click);
			// 
			// pauseButton
			// 
			this->pauseButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->pauseButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pauseButton.Image")));
			this->pauseButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->pauseButton->Name = L"pauseButton";
			this->pauseButton->Size = System::Drawing::Size(23, 22);
			this->pauseButton->Text = L"Pause";
			this->pauseButton->Click += gcnew System::EventHandler(this, &Playback::pauseButton_Click);
			// 
			// playButton
			// 
			this->playButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->playButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"playButton.Image")));
			this->playButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->playButton->Name = L"playButton";
			this->playButton->Size = System::Drawing::Size(23, 22);
			this->playButton->Text = L"Play";
			this->playButton->Click += gcnew System::EventHandler(this, &Playback::playButton_Click);
			// 
			// Record
			// 
			this->Record->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->Record->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"Record.Image")));
			this->Record->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->Record->Name = L"Record";
			this->Record->Size = System::Drawing::Size(23, 22);
			this->Record->Text = L"Record";
			this->Record->Click += gcnew System::EventHandler(this, &Playback::RecordButton_Click);
			// 
			// videoPanel
			// 
			this->videoPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->videoPanel->Location = System::Drawing::Point(12, 73);
			this->videoPanel->Name = L"videoPanel";
			this->videoPanel->Size = System::Drawing::Size(102, 184);
			this->videoPanel->TabIndex = 1;
			// 
			// timeTrackBar
			// 
			this->timeTrackBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->timeTrackBar->AutoSize = false;
			this->timeTrackBar->Location = System::Drawing::Point(0, 285);
			this->timeTrackBar->Margin = System::Windows::Forms::Padding(0);
			this->timeTrackBar->Name = L"timeTrackBar";
			this->timeTrackBar->Size = System::Drawing::Size(310, 20);
			this->timeTrackBar->TabIndex = 4;
			this->timeTrackBar->Scroll += gcnew System::EventHandler(this, &Playback::timeTrackBar_Scroll);
			// 
			// monthCalendar
			// 
			this->monthCalendar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->monthCalendar->Location = System::Drawing::Point(126, 102);
			this->monthCalendar->Name = L"monthCalendar";
			this->monthCalendar->TabIndex = 5;
			this->monthCalendar->DateChanged += gcnew System::Windows::Forms::DateRangeEventHandler(this, &Playback::monthCalendar_DateChanged);
			// 
			// dateTimePicker
			// 
			this->dateTimePicker->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->dateTimePicker->CustomFormat = L"dd MMM yyyy";
			this->dateTimePicker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker->Location = System::Drawing::Point(127, 75);
			this->dateTimePicker->Name = L"dateTimePicker";
			this->dateTimePicker->ShowUpDown = true;
			this->dateTimePicker->Size = System::Drawing::Size(144, 20);
			this->dateTimePicker->TabIndex = 3;
			this->dateTimePicker->ValueChanged += gcnew System::EventHandler(this, &Playback::dateTimePicker_ValueChanged);
			// 
			// showCalendarCheckBox
			// 
			this->showCalendarCheckBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->showCalendarCheckBox->Appearance = System::Windows::Forms::Appearance::Button;
			this->showCalendarCheckBox->Font = (gcnew System::Drawing::Font(L"Webdings", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(2)));
			this->showCalendarCheckBox->Location = System::Drawing::Point(279, 73);
			this->showCalendarCheckBox->Margin = System::Windows::Forms::Padding(0);
			this->showCalendarCheckBox->Name = L"showCalendarCheckBox";
			this->showCalendarCheckBox->Size = System::Drawing::Size(25, 25);
			this->showCalendarCheckBox->TabIndex = 7;
			this->showCalendarCheckBox->Text = L"6";
			this->showCalendarCheckBox->UseVisualStyleBackColor = true;
			this->showCalendarCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Playback::showCalendarCheckBox_CheckedChanged);
			// 
			// trackBarTimer
			// 
			this->trackBarTimer->Tick += gcnew System::EventHandler(this, &Playback::trackBarTimer_Tick);
			// 
			// timeStampLabel
			// 
			this->timeStampLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->timeStampLabel->AutoSize = true;
			this->timeStampLabel->Location = System::Drawing::Point(12, 266);
			this->timeStampLabel->Name = L"timeStampLabel";
			this->timeStampLabel->Size = System::Drawing::Size(0, 13);
			this->timeStampLabel->TabIndex = 8;
			// 
			// stillImagesFolderButton
			// 
			this->stillImagesFolderButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->stillImagesFolderButton->Location = System::Drawing::Point(280, 38);
			this->stillImagesFolderButton->Name = L"stillImagesFolderButton";
			this->stillImagesFolderButton->Size = System::Drawing::Size(24, 22);
			this->stillImagesFolderButton->TabIndex = 10;
			this->stillImagesFolderButton->Text = L"...";
			this->stillImagesFolderButton->UseVisualStyleBackColor = true;
			this->stillImagesFolderButton->Click += gcnew System::EventHandler(this, &Playback::stillImagesFolderButton_Click);
			// 
			// stillImagesFolderTextBox
			// 
			this->stillImagesFolderTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->stillImagesFolderTextBox->Location = System::Drawing::Point(93, 38);
			this->stillImagesFolderTextBox->Name = L"stillImagesFolderTextBox";
			this->stillImagesFolderTextBox->Size = System::Drawing::Size(178, 20);
			this->stillImagesFolderTextBox->TabIndex = 9;
			this->stillImagesFolderTextBox->TextChanged += gcnew System::EventHandler(this, &Playback::stillImagesFolderTextBox_TextChanged);
			// 
			// stillImagesDirectoryLabel
			// 
			this->stillImagesDirectoryLabel->AutoSize = true;
			this->stillImagesDirectoryLabel->Location = System::Drawing::Point(4, 42);
			this->stillImagesDirectoryLabel->Name = L"stillImagesDirectoryLabel";
			this->stillImagesDirectoryLabel->Size = System::Drawing::Size(88, 13);
			this->stillImagesDirectoryLabel->TabIndex = 11;
			this->stillImagesDirectoryLabel->Text = L"Still images folder";
			// 
			// Playback
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(310, 317);
			this->Controls->Add(this->stillImagesFolderButton);
			this->Controls->Add(this->stillImagesFolderTextBox);
			this->Controls->Add(this->stillImagesDirectoryLabel);
			this->Controls->Add(this->timeStampLabel);
			this->Controls->Add(this->showCalendarCheckBox);
			this->Controls->Add(this->monthCalendar);
			this->Controls->Add(this->timeTrackBar);
			this->Controls->Add(this->dateTimePicker);
			this->Controls->Add(this->videoPanel);
			this->Controls->Add(this->toolStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"Playback";
			this->Text = L"Playback";
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->timeTrackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void playButton_Click(System::Object^  sender, System::EventArgs^  e) {
				this->trackBarTimer->Start();
				if(m_pJpegReader)
					m_pJpegReader->Run();
			 }
private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e) {
				StopPlayback();
				//this->trackBarTimer->Stop();
		 }
private: System::Void pauseButton_Click(System::Object^  sender, System::EventArgs^  e) {
				if(m_pJpegReader)
					m_pJpegReader->Pause();
		 }
private: System::Void dateTimePicker_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 m_pJpegReader->Stop();
			 this->monthCalendar->SetDate(this->dateTimePicker->Value);
			 
			 SetStillFolder();
		 }
private: System::Void monthCalendar_DateChanged(System::Object^  sender, System::Windows::Forms::DateRangeEventArgs^  e) {
			 this->dateTimePicker->Value = this->monthCalendar->SelectionStart;
			 //this->monthCalendar->Hide();
		 }
private: System::Void showCalendarCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(this->showCalendarCheckBox->Checked)
				 this->monthCalendar->Show();
			 else
				 this->monthCalendar->Hide();
		 }
private: System::Void trackBarTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
			 REFERENCE_TIME timeNow = 0;
			 m_pJpegReader->GetCurrentPosition(&timeNow);
			 this->timeTrackBar->Value = ((LONG)(timeNow / ONE_MSEC));

			 CHAR fileName[MAX_PATH];
			 if(S_OK == m_pJpegReader->GetCurrentFileName(fileName, MAX_PATH))
				 this->timeStampLabel->ForeColor = System::Drawing::Color::Black;
			 else
				 this->timeStampLabel->ForeColor = System::Drawing::Color::Red;

			 this->timeStampLabel->Text = gcnew String(fileName);
		 }
private: System::Void timeTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
			if(m_pJpegReader->CanSeek())
			{
				REFERENCE_TIME pos = ONE_MSEC * (REFERENCE_TIME)this->timeTrackBar->Value;
				m_pJpegReader->SetPosition(&pos);
			}
		 }
private: System::Void stillImagesFolderButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(this->stillImagesFolderTextBox->Text != "")
				this->stillFolderBrowserDialog->SelectedPath = this->stillImagesFolderTextBox->Text;
			 if(this->stillFolderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 	this->stillImagesFolderTextBox->Text = stillFolderBrowserDialog->SelectedPath;
		 }
private: System::Void stillImagesFolderTextBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 BrowseStillFolder();
		 }
private: System::Void RecordButton_Click(System::Object^  sender, System::EventArgs^  e) {
				System::Windows::Forms::DialogResult res = this->saveFileDialog->ShowDialog();
				if(res == System::Windows::Forms::DialogResult::OK)
				{
					DbgPrint("dialog returned OK, filename = %s", this->saveFileDialog->FileName);
					if(!m_pJpegReader)
						return;
					
					//m_pJpegReader->Disconnect();
					StopPlayback();
					m_pJpegReader->DeleteFilters();

					// display "recording" message, disable controls
					// Connect recorder
					WCHAR* name = (WCHAR*)(void*)Marshal::StringToHGlobalUni(this->saveFileDialog->FileName);
					m_pJpegReader->BuildRecordingGraph(name);
					Marshal::FreeHGlobal(IntPtr((void*)name));
					// record
					// Set directory
					SetStillFolder();
					m_pJpegReader->Run();
					// wait
					HANDLE m_hDoneEvent = NULL;
					m_pJpegReader->GetCompleteEvent(&m_hDoneEvent);
					WaitForSingleObject(m_hDoneEvent, INFINITE);
					MessageBox::Show("Recording completed", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
					// disconnect
					StopPlayback();

					//m_pJpegReader->Disconnect();
					m_pJpegReader->DeleteFilters();

					//reconnect renderer
					m_pJpegReader->BuildGraph();
					m_pJpegReader->SetOwner(reinterpret_cast<HWND>(this->videoPanel->Handle.ToPointer()), 0, 0, 640, 480);
					// Set directory again
					SetStillFolder();

					// enable controls
				}
		 }
};
}
