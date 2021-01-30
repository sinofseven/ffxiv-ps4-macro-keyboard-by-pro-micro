use serde::{Deserialize, Serialize};

#[derive(Debug, Deserialize)]
pub enum MacroKind {
    TimeLimit,
    CountLimit,
}

#[derive(Debug, Deserialize)]
pub struct MacroStep {
    pub key: String,
    pub interval: u64,
}

#[derive(Debug, Deserialize)]
pub struct MacroConfig {
    pub name: String,
    pub description: Option<String>,
    pub kind: MacroKind,
    pub limit: u64,
    pub steps: Vec<MacroStep>,
}

#[derive(Debug, Deserialize)]
pub struct ConfigFile {
    pub macros: Vec<MacroConfig>,
}

#[derive(Debug, Serialize)]
pub struct RenderModel {
    limit: u64,
    key_numbers: usize,
    keys: String,
    intervals: String,
}

impl MacroConfig {
    pub fn create_render_model(&self) -> RenderModel {
        let keys: Vec<String> = self.steps.iter().map(|s| s.key.clone()).collect();
        let intervals: Vec<String> = self.steps.iter().map(|s| s.interval.to_string()).collect();

        RenderModel {
            limit: self.limit,
            key_numbers: self.steps.len(),
            keys: keys.join(", "),
            intervals: intervals.join(", "),
        }
    }
}

impl ConfigFile {
    pub fn new() -> Result<ConfigFile, String> {
        let raw = std::fs::read_to_string("macros.yml")
            .map_err(|e| format!("Error occurred: {:?}", e))?;
        let config: ConfigFile = serde_yaml::from_str(&raw)
            .map_err(|e| format!("Error occurred in load config file: {:?}", e))?;
        Ok(config)
    }
}
