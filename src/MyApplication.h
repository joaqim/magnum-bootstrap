#pragma once

/* Base UI plane */
/*
constexpr Vector2 WidgetSize{80, 32};
const std::regex FloatValidator{R"(-?\d+(\.\d+)?)"};

struct BaseUiPlane: Ui::Plane {
  explicit BaseUiPlane(Ui::UserInterface& ui):
  Ui::Plane{ui, Ui::Snap::Top|Ui::Snap::Bottom|Ui::Snap::Left|Ui::Snap::Right, 0, 16, 128},
      metalness{*this, {Ui::Snap::Top|Ui::Snap::Right, WidgetSize}, FloatValidator, "0.5", 5},
          roughness{*this, {Ui::Snap::Bottom, metalness, WidgetSize}, FloatValidator, "0.25", 5},
              f0{*this, {Ui::Snap::Bottom, roughness, WidgetSize}, FloatValidator, "0.25", 5},

                  apply{*this, {Ui::Snap::Bottom|Ui::Snap::Right, WidgetSize}, "Apply", Ui::Style::Primary},
                      reset{*this, {Ui::Snap::Top, apply, WidgetSize}, "Reset", Ui::Style::Danger}
                      {
                        Ui::Label{*this, {Ui::Snap::Left, metalness}, "Metalness", Text::Alignment::MiddleRight};
                        Ui::Label{*this, {Ui::Snap::Left, roughness}, "Roughness", Text::Alignment::MiddleRight};
                        Ui::Label{*this, {Ui::Snap::Left, f0}, "ƒ₀", Text::Alignment::MiddleRight};

                        Ui::Label{*this, {Ui::Snap::Bottom|Ui::Snap::Left, WidgetSize},
                              "Use WASD + mouse to move, (Shift +) M/R/F to change parameters.",
									                Text::Alignment::MiddleLeft};
                      }

                      Ui::ValidatedInput metalness,
                          roughness,
                          f0;
                      Ui::Button apply,
                          reset;
};

*/