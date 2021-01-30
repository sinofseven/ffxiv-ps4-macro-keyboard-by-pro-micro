use crate::models::{MacroConfig, MacroKind};
use tinytemplate::TinyTemplate;

const TEMPLATE_TIME_LIMIT: &str = include_str!("time_limit_macro.ino");
const TEMPLATE_COUNT_LIMIT: &str = include_str!("count_limit_macro.ino");
const KIND_TIME_LIMIT: &str = "TimeLimit";
const KIND_COUNT_LIMIT: &str = "CountLimit";

pub struct Render<'template> {
    te: TinyTemplate<'template>,
}

impl Render<'_> {
    pub fn new() -> Result<Render<'static>, String> {
        let mut render = Render {
            te: TinyTemplate::new(),
        };
        render
            .te
            .add_template(KIND_TIME_LIMIT, TEMPLATE_TIME_LIMIT)
            .map_err(|e| format!("Error occurred in adding time limit tempalte: {:?}", e))?;
        render
            .te
            .add_template(KIND_COUNT_LIMIT, TEMPLATE_COUNT_LIMIT)
            .map_err(|e| format!("Error occurred in adding count limit tempalte: {:?}", e))?;
        Ok(render)
    }

    pub fn rend(&self, config: &MacroConfig) -> Result<String, String> {
        let model = config.create_render_model();
        let kind = match config.kind {
            MacroKind::TimeLimit => KIND_TIME_LIMIT,
            MacroKind::CountLimit => KIND_COUNT_LIMIT,
        };
        self.te
            .render(kind, &model)
            .map_err(|e| format!("Error occurred in rendering: {:?}", e))
    }
}
