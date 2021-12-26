use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


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
	if index ==0{
		continue;
	}
	else{
		let a :i32 = num[0].trim().parse().unwrap();
		let b :i32 = num[1].trim().parse().unwrap();
		if a<0 || a>10000 || b<0 || b>10000{
			write!(outfile, "#{} -1\n",index).ok();
		}
		else{
			let mut x : i32 = 0;
			let mut y : i32 = 0;
			for i in 1..a{
				let mut count :i32 =0;
				for j in 1..i{
					if i%j==0{
						count +=1;
					}
				}
				if count==1 {
					x +=1;
				}
			}
			for k in 1..b{
				let mut count :i32=0;
				for l in 1..k{
					if k%l==0{
						count +=1;
					}
				}
				if count ==1 {
					y +=1;
				}
			}
			println!("#{} {}", index, y-x);
			write!(outfile , "#{} {}\n", index , y-x).ok();
		}
	}
   }
}
