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
	if index==0{
		continue;
	}
	else{
		let a :i64 = num[0].trim().parse().unwrap();
		if a<10 || a>99999 {
			write!(outfile,"#{} -1\n", index).ok();
			println!("#{} -1",index);
		}
		else{
			let b = (a as f64).sqrt() as f64;
			let c = (a as f64).sqrt() as i64;
			let mut x:i32=0;
			if b==c as f64{
				if a >10000{
					x=two(a,c);
				}
				else if a >1000{
					if a==10000{
						x+=1;
					}
					else{
						x=one(a,c);
					}
				}
				else if a >100{
					if a==100{
						x+=1;
					}
				}
				else if a >10{
					if a==81||a==100{
						x+=1;
					}
				}
				if x>0 {
					println!("#{} YES", index);
					write!(outfile, "#{} YES\n", index).ok();
				}
				else{
					println!("#{} NO", index);
					write!(outfile, "#{} NO\n", index).ok();
				}
        		}
        		else{
				println!("#{} NO", index);
				write!(outfile, "#{} NO\n", index).ok();
        		}
        	}
        }
   }
}
fn one(a: i64,b: i64) -> i32 {
    let mut x:i64;
    let mut y:i64;
    let mut z:i64;
    let mut k:i32=0;
    x=a/100;y=(a%100)/10;z=a%10;
    println!("x: {} y: {} z: {}",x,y,z);
    if x+y+z==b{k+=1;}
    // 1/2/1
    x=a/1000;y=(a%1000)/10;z=a%10;
    println!("x: {} y: {} z: {}",x,y,z);
    if x+y+z==b{k+=1;}
    //1/1/2
    x=a/1000;y=(a%1000)/100;z=a%100;
    println!("x: {} y: {} z: {}",x,y,z);
    if x+y+z==b{k+=1;}
    x=a/100;y=a%100;z=0;
    if x+y+z==b{k+=1;}
    return k;
}
fn two(a: i64,b: i64) -> i32 {
    let mut x:i64;
    let mut y:i64;
    let mut z:i64;
    let mut k:i32=0;
    x=a/1000;y=(a%1000)/10;z=a%10;
    if x+y+z==b{k+=1;}
    // 2/1/2
    x=a/1000;y=(a%1000)/100;z=a%100;
    if x+y+z==b{k+=1;}
    // 1/2/2
    x=a/10000;y=(a%10000)/100;z=a%100;
    if x+y+z==b{k+=1;}
    // 3/1/1
    x=a/100;y=(a%100)/10;z=a%10;
    if x+y+z==b{k+=1;}
    // 1/3/1
    x=a/10000;y=(a%10000)/10;z=a%10;
    if x+y+z==b{k+=1;}
    // 1/1/3
    x=a/10000;y=(a%10000)/1000;z=a%1000;
    if x+y+z==b{k+=1;}
    // 3/2
    x=a/100;y=a%100;
    if x+y==b{k+=1;}
    // 2/3
    x=a/1000;y=a%1000;
    if x+y==b{k+=1;}
    return k;
}
//3025 = 55^2
//30+25
