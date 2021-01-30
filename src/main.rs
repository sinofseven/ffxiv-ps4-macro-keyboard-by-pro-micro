mod models;
mod render;
mod writer;

use models::ConfigFile;
use render::Render;
use writer::Content;

fn main() {
    let result = process();
    if let Err(e) = result {
        eprintln!("{}", e);
        std::process::exit(1);
    }
}

fn process() -> Result<(), String> {
    let config = ConfigFile::new()?;
    let render = Render::new()?;

    let mut contents: Vec<Content> = Vec::new();
    for config in config.macros {
        contents.push(Content {
            name: config.name.clone(),
            code: render.rend(&config)?,
        });
    }

    let result = std::fs::remove_dir_all("arduino");

    if let Err(e) = result {
        match e.kind() {
            std::io::ErrorKind::NotFound => {}
            _ => return Err(format!("Error occurred in delete present: {:?}", e)),
        }
    }

    for content in contents {
        content.write()?;
    }
    Ok(())
}
