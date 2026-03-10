using Githubhacktues12please.Entities;

namespace Githubhacktues12please.Services
{
    public class MultiplyService : IMultiplyService
    {
        public Hz MultyplyHz(float Hz, float multiplier)
        {
            float NewHz = Hz * multiplier;

            return new Hz
            {
                hz = NewHz
            };
        }
    }
}
