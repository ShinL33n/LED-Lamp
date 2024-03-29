using Microsoft.AspNetCore.Components;
using Microsoft.JSInterop;

namespace LED_Lamp_Controller_App.Pages
{
    public partial class TimeFrame
    {
        [Inject]
        private IJSRuntime JSRuntime { get; set; }

        private static int topLeftSliderValue { get; set; }
        private static int topRightSliderValue { get; set; }
        private static int bottomLeftSliderValue { get; set; }
        private static int bottomRightSliderValue { get; set; }
        private static int brightnessSliderValue { get; set; }

        private async Task HandleBrightnessSliderInput(ChangeEventArgs e)
        {
            await JSRuntime.InvokeVoidAsync("setTopSlidersPosition");
        }

        private async Task HandleTopSliderInput(ChangeEventArgs e)
        {
            await JSRuntime.InvokeVoidAsync("changeMiddleHoursLineWidth");

            //int.Parse(e.Value.ToString());
            //HandleBottomSliderInput();
        }

        private void UpdateBottomLeftSlider(ChangeEventArgs e)
        {
            bottomLeftSliderValue = int.Parse(e.Value.ToString());
            HandleBottomSliderInput();

        }

        private void UpdateBottomRightSlider(ChangeEventArgs e)
        {
            bottomRightSliderValue = int.Parse(e.Value.ToString());
            HandleBottomSliderInput();
        }

        private void HandleBottomSliderInput()
        {
            if(bottomLeftSliderValue < topLeftSliderValue)
                topLeftSliderValue = bottomLeftSliderValue;
            if (bottomRightSliderValue < topRightSliderValue)
                topRightSliderValue = bottomRightSliderValue;
            
            // uwagi:
            // teraz dziala to na odwrot jak chcialem, do zrobienia zeby dzialalo jeszce w druga strone
            // oraz updatowało długośc linii
            // PRZYKŁAD W indivudual leds
        }
    }
}
