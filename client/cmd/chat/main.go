package main

import (
	"fmt"
	"os"

	"github.com/ThreeLD/pgin/client/internal/tui"
	tea "github.com/charmbracelet/bubbletea"
)

func main() {
	p := tea.NewProgram(
		tui.NewModel(),
		tea.WithAltScreen(),       // Enable alternative screen buffer
		tea.WithMouseCellMotion(), // Enable mouse events
	)
	if _, err := p.Run(); err != nil {
		fmt.Println("Error running program:", err)
		os.Exit(1)
	}
}
