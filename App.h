#pragma once
#include "APICore.h"

class App {
public:
	App();
	~App();

	int Run(HINSTANCE hInst, int nCmdShow);

private:
	// Controls id
	enum ID_CONTROLS {
		ID_CLOSE_BUTTON = 1001,
		ID_COLLAPSE_BUTTON = 1002,
		ID_CONVERT_BUTTON = 1003,
		ID_FROM = 1006,
		ID_TO = 1007
	};
	

	// Handles for buttons
	HWND hCloseButton, hCollapseButton, hConvertButton;

	// Handles for text fields
	HWND hFirstEdit, hSecondEdit;

	// Handles for dropdown lists
	HWND hFirstComboBox, hSecondComboBox;

// Button press tracking
private:
	void onCloseButtonClicked();
	void onCollapseButtonClicked();
	void onConvertButtonClicked();

// To draw custom controls
private:
	void DrawCloseButton(HDC hdc);
	void DrawCollapseButton(HDC hdc);
	void DrawConvertButton(HDC hdc);

// To add items to a dropdown list
private:
	void addItemsToFirstComboBox();
	void addItemsToSecondComboBox();

	void getItemsFromComboBox(HWND hComboBox, wchar_t** buffer);


// Button press tracking
private:
	bool bCloseButtonPressed;
	bool bCollapseButtonPressed;
	bool bConvertButtonPressed;

// To render controls
private:
	LPDRAWITEMSTRUCT pdis;

// Getting currency value
private:
	float usdToRub;
	float eurToRub;
	float qarToRub;
	float eurToUsd;
	float qarToUsd;
	float eurToQar;

private:

	// Window message procedure
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Static application message procedure
	static LRESULT CALLBACK ApplicationProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Window Class
	WNDCLASS wc;

	// Handle to main window
	HWND hwnd;

	void onWindowPaint();

private:
	// To create controls
	void CreateNativeControls();
};