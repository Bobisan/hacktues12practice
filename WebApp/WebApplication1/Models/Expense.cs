using System.ComponentModel.DataAnnotations;

namespace WebApplication1.Models;

public class Expense
{
    
    public decimal Value { get; set; }
    
    [Required]
    
    public string? Description { get; set; }
}