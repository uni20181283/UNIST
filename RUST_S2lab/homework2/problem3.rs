use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_3.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_3.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
		if index==0 {
		continue;
	}
	else {
		let mut a :i32 = num[0].trim().parse().unwrap();
		let mut b :i32 = num[1].trim().parse().unwrap();
		if a<0 || a>1000000 || b<0 || b>1000000{
			write!(outfile,"#{} -1\n",index).ok();
		}
		else{
		let mut c:i32=1;
		let mut d:i32=1;
		while a!=1{
			if a%2 ==0 {
				a = a/2;
				c +=1;
			}
			else {
				a = 3*a+1;
				c +=1;
			}
		}
		while b!=1{
			if b%2 ==0 {
				b = b/2;
				d +=1;
			}
			else {
				b = 3*b+1;
				d +=1;
			}
		}
		if c>d {
			println!("#{} {}", index,c);
			write!(outfile, "#{} {}\n", index ,c).ok();
		}
		else {
			println!("#{} {}", index, d);
			write!(outfile, "#{} {}\n", index, d).ok();
		}
		}
        }
        }
}
//retey//retey//retey//retey//retey//retey//retey//retey//retey//retey//retey//retey
