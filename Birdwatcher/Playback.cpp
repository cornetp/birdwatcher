#include "StdAfx.h"
#include "Playback.h"
#include "Debug.h"

namespace Birdwatcher {

void Playback::InitForm(void)
{
	m_pJpegReader = new CJpegReader();
	if(m_pJpegReader == NULL || m_pJpegReader->BuildGraph() != NOERROR)
	{
		MessageBox::Show("Cannot create readback filter graph", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		this->Close();
		return;
	}

	//this->stillImagesFolderTextBox->Text = gcnew String("C:\\Documents and Settings\\cornetp\\My Documents\\tmp\\still");
	CHAR stillFolder[MAX_PATH];
	m_pGraph->GetStillDirectory(stillFolder);
	if(strcmp(stillFolder, "") == 0)
		this->stillImagesFolderTextBox->Text = gcnew String(System::IO::Directory::GetCurrentDirectory());
	else
		this->stillImagesFolderTextBox->Text = gcnew String(stillFolder);

	//BrowseStillFolder();
	this->monthCalendar->Hide();

	// TODO: Get image size
	int w = 640;
	int h = 480;

	int incX = w-this->videoPanel->Size.Width;
	int incY = h-this->videoPanel->Size.Height;
	this->videoPanel->Size = System::Drawing::Size(w, h);

	this->ClientSize = System::Drawing::Size(this->ClientSize.Width+incX, this->ClientSize.Height+incY);
	//this->dateTimePicker->Location = System::Drawing::Point(this->dateTimePicker->Location.X + incX, this->dateTimePicker->Location.Y);

	// track bar
	LONGLONG rtDuration = 0;
	m_pJpegReader->GetDuration(&rtDuration);
	this->timeTrackBar->SetRange(0, (int)(rtDuration/ONE_MSEC));

	this->trackBarTimer->Interval = TICK_FREQ;

	m_pJpegReader->SetOwner(reinterpret_cast<HWND>(this->videoPanel->Handle.ToPointer()), 0, 0, w, h);
}

void Playback::BrowseStillFolder()
{
	DbgPrint("BrowseStillFolder");
	// calendar
	String ^folder = this->stillImagesFolderTextBox->Text;
	System::Collections::Generic::IEnumerable<String^>^ dirs = Directory::EnumerateDirectories(folder);
	System::Collections::Generic::IEnumerator<String^>^ en = dirs->GetEnumerator();
	String^ path;
	DateTime date;

	bool bOK = false;
	while(en->MoveNext())
	{
		path = en->Current;
		String^ dir = Path::GetFileName(path);
		try
		{
			date = DateTime::ParseExact(dir, "yyyyMMdd", CultureInfo::InvariantCulture); 
			this->monthCalendar->AddBoldedDate(date);
			bOK = true;
		}
		catch(Exception^ e)
		{
			DbgPrint("Incorrect folder %s: %s", dir, e->Message);
			StopPlayback();
		}
		//DbgPrint("dir = %s: date: %02d / %02d / %04d", dir, date.Day, date.Month, date.Year);
	}
	if(bOK)
	{
		if(date == this->dateTimePicker->Value)
		{
			SetStillFolder();
		}
		else
		{
			this->monthCalendar->SetDate(date);
			this->dateTimePicker->Value = date;
		}

		this->timeStampLabel->Text = "";
		this->timeStampLabel->ForeColor = System::Drawing::Color::Black;
	}
	else
	{
		this->timeStampLabel->Text =  gcnew String("No still subfolder of the type \"yyyyMMdd\" has been found in the selected directory");
		this->timeStampLabel->ForeColor = System::Drawing::Color::Red;
	}
}

void Playback::SetStillFolder()
{
	DbgPrint("SetStillFolder");
	//this->trackBarTimer->Stop();
	StopPlayback();
	System::DateTime date = this->dateTimePicker->Value;

	CHAR dir[MAX_PATH];
	sprintf_s(dir, MAX_PATH, "%s\\%04d%02d%02d",
		this->stillImagesFolderTextBox->Text, date.Year, date.Month, date.Day);

	m_pJpegReader->SetStillDirectory(dir);

	// Update trackbar
	LONGLONG rtDuration = 0;
	m_pJpegReader->GetDuration(&rtDuration);
	this->timeTrackBar->SetRange(0, (int)(rtDuration/ONE_MSEC));
	//this->trackBarTimer->Start();
}

void Playback::StopPlayback()
{
	if(m_pJpegReader)
	{
		m_pJpegReader->Stop();

		if(m_pJpegReader->CanSeek())
		{
			REFERENCE_TIME pos = 0;
			m_pJpegReader->SetPosition(&pos);
		}
	}
	this->trackBarTimer->Stop();
	this->timeTrackBar->Value = 0;
	}
};
