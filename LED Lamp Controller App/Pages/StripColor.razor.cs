using Microsoft.AspNetCore.Components;

namespace LED_Lamp_Controller_App.Pages
{
    public partial class StripColor
    {
        private static int RedSliderCurrentValue {  get; set; }
        private static int GreenSliderCurrentValue { get; set; }
        private static int BlueSliderCurrentValue { get; set; }
        private static int WhiteSliderCurrentValue { get; set; }

        private string? RedSliderCurrentValueText { get; set; }
        private string? GreenSliderCurrentValueText { get; set; }
        private string? BlueSliderCurrentValueText { get; set; }
        private string? WhiteSliderCurrentValueText { get; set; }



        private void HandleTextInput(ChangeEventArgs e, string color)
        {
            if (int.TryParse(e.Value.ToString(), out int newValue))
            {
                if (newValue < 0)
                {
                    newValue = 0;
                }
                else if (newValue > 255)
                {
                    newValue = 255;
                }

                switch (color)
                {
                    case "Red":
                        RedSliderCurrentValue = newValue;
                        RedSliderCurrentValueText = newValue.ToString();
                        break;
                    case "Green":
                        GreenSliderCurrentValue = newValue;
                        GreenSliderCurrentValueText = newValue.ToString();
                        break;
                    case "Blue":
                        BlueSliderCurrentValue = newValue;
                        BlueSliderCurrentValueText = newValue.ToString();
                        break;
                    case "White":
                        WhiteSliderCurrentValue = newValue;
                        WhiteSliderCurrentValueText = newValue.ToString();
                        break;
                }
            }
            else
            {
                RedSliderCurrentValueText = RedSliderCurrentValue.ToString();
            }
        }

    }
}
