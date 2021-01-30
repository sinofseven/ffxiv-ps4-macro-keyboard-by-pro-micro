pub struct Content {
    pub name: String,
    pub code: String,
}

impl Content {
    pub fn write(&self) -> Result<(), String> {
        let file_name = format!("{}.ino", self.name);
        let file_dir_path = format!("arduino/{}", self.name);
        let file_path = format!("arduino/{}/{}", self.name, file_name);
        std::fs::create_dir_all(file_dir_path)
            .map_err(|e| format!("Error occurred in create directory: {:?}", e))?;
        std::fs::write(file_path, &self.code)
            .map_err(|e| format!("Error occurred in write code: {:?}", e))
    }
}
