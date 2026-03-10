using Githubhacktues12please.Entities;
using Githubhacktues12please.Models;
using Githubhacktues12please.Models.DTOs;
using Githubhacktues12please.Services;
using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Diagnostics;

namespace Githubhacktues12please.Controllers
{
    public class HomeController : Controller
    {
        private MySqlConnection con = new MySqlConnection("SERVER=10.210.66.98;DATABASE=hacktues_practice;UID=Burgaski_Glarusi; PASSWORD=Dildoto_n@_pepelqshk4;"); 
        private readonly IMultiplyService _multiplierService;
        private float Hz = 1;

        public HomeController(IMultiplyService multiplyService)
        {
            _multiplierService = multiplyService;
        }

        public IActionResult Index(MultiplierDTO dto)
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [HttpPost]
        public IActionResult UpdateHz(MultiplierDTO dto)
        {
            con.Open();
            Hz result = _multiplierService.MultyplyHz(Hz, dto.Multiplier);
            MySqlCommand cmd = new MySqlCommand("INSERT INTO `data` (value, description) VALUES (@value, @description);", con);
            cmd.Parameters.AddWithValue("@value", result.hz);
            cmd.Parameters.AddWithValue("@description", $"Multiplied {Hz} by {dto.Multiplier} to get {result.hz}");
            cmd.ExecuteNonQuery();
            con.Close();
            return RedirectToAction("Index");
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
