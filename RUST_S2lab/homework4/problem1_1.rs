use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};
use


fn exchange(a: &mut i32, b: &mut i32) {
	let c=a.clone();
	//println!("a:{} b:{} c:{}",a,b,c);
	*a=b.clone();
	*b=c;
	//println!("a:{} b:{} c:{}",a,b,c);
}

fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_1.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_1.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let mut a :i32 = num[0].trim().parse().unwrap();
		let mut b :i32 = num[1].trim().parse().unwrap();
		//println!("{} {}",a,b);
		if a< -1000 || a>1000 || b< -1000 || b>1000{
			write!(outfile,"#{} -1\n",index).ok();
		}
		else{
			write!(outfile,"#{} {} {}",index,a,b).ok();
			print!("#{} {} {}",index,a,b);
			exchange(&mut a,&mut b);
			write!(outfile, " {} {}\n", a,b).ok();
			println!(" {} {}", a,b);
		}
        }
   }  
}
