using System.Data;
using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using WebApplication1.Models;
using MySql.Data.MySqlClient;

namespace WebApplication1.Controllers;

public class HomeController : Controller
{
    private MySqlConnection con = new MySqlConnection("SERVER=10.210.66.98;DATABASE=hacktues_practice;UID=Burgaski_Glarusi; PASSWORD=Dildoto_n@_pepelqshk4;");
    
    private readonly ILogger<HomeController> _logger;

    public HomeController(ILogger<HomeController> logger)
    {
        _logger = logger;
    }

    public IActionResult Index()
    {
        return View();
    }

    public IActionResult Expenses()
    {
        return View();
    }

    public IActionResult CreateExpense()
    {
        return View();
    }

    public IActionResult CreateExpenseForm(Expense model)
    {
        try
        {
            con.Open();
            Console.WriteLine("Connected to MySQL!");
            string createTableQuery = @"
            CREATE TABLE IF NOT EXISTS data (
                Id INT AUTO_INCREMENT PRIMARY KEY,
                value DECIMAL(3,2) NOT NULL,
                description VARCHAR(100)
            );";
            
            MySqlCommand cmd = new MySqlCommand(createTableQuery, con);
            cmd.ExecuteNonQuery();

            Console.WriteLine("Table created!");
            
            string insertQuery = "INSERT INTO data (value, description) VALUES (@value, @description)";

            MySqlCommand insertCmd = new MySqlCommand(insertQuery, con);
            insertCmd.Parameters.AddWithValue("@value", model.Value);
            insertCmd.Parameters.AddWithValue("@description", model.Description);

            insertCmd.ExecuteNonQuery();

            Console.WriteLine("Data inserted!");
            
            con.Close();
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex.Message);
        }
        return RedirectToAction("Expenses");
    }

    public IActionResult Privacy()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}