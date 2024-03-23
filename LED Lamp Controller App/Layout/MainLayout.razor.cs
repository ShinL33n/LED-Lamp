using Microsoft.AspNetCore.Components;
using Microsoft.JSInterop;

namespace LED_Lamp_Controller_App.Layout
{
    public partial class MainLayout
    {
        [Inject]
        private IJSRuntime? JSRuntime { get; set; }

        private readonly string keyDarkMode = "DarkMode";

        private bool darkModeToggled = false;
        private string? DarkModeCssClass => darkModeToggled ? "dark-mode" : null;


        protected override async Task OnInitializedAsync()
        {
            string darkModeSateFromLocalStorage = await JSRuntime.InvokeAsync<string>("readLocalStorage", keyDarkMode);
            darkModeToggled = !string.IsNullOrEmpty(darkModeSateFromLocalStorage) && darkModeSateFromLocalStorage == "true" ? true : false;
        }
        private async void ToggleDarkMode()
        {
            darkModeToggled = !darkModeToggled;
            await JSRuntime.InvokeVoidAsync("addToLocalStorage", keyDarkMode, darkModeToggled);
        }

    }
}
